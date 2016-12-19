#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <sstream>
#include <fstream>


#include "main.h"
#include "buttons.h"
#include "display.h"
#include "tcp_server.h"
#include "tcp_client.h"


using namespace std;
 
#define FAIL  -1

int server;
SSL *temp_ssl;
int players=1;
int send_count=-1;
char ip[10000];
bool players_written =false;

int OpenListener(int port)
{   int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
 
SSL_CTX* InitServerCTX(void)
{   const SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = SSLv3_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}


 
void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{   char buf[10000];
    char reply[10000];
    int sd, bytes;
    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
    {
        ERR_print_errors_fp(stderr);
       }
    else
    {
        bytes = SSL_read(ssl, buf, 10000); /* get request */
        buf[bytes] = 0;
        if ( bytes > 0 )
        {
            printf("Received Client IP and username: %s\n", buf);
            char *pch;
            pch = strtok(buf," ");
            ipaddress[players]=pch;
            pch=strtok(NULL," ");
            playername[players]=pch;
            printf("Sending player %d id\n",players+1);
            sprintf(reply, "%d", players+1);   /* construct reply */
            SSL_write(ssl, reply, strlen(reply)); /* send reply */
			players++;
			if (players==6) {send_count=0;}
        }
        else
           { ERR_print_errors_fp(stderr); exit(0); }
        while (!send_difficulty) {};
        close(server);
        printf("send_difficulty is now true\n");
			while (send_count<0) {};
			stringstream ss;
			ss << TOTAL_PLAYERS;
			string temp_buf = ss.str();
			sprintf(reply, temp_buf.c_str(), buf);
			SSL_write(ssl, reply, strlen(reply));
			printf("total players sent\n");
			strcpy(reply,"");
			for (int i=0;i<6;i++)
			{strcat(reply,ipaddress[i].c_str());
			strcat(reply," ");
			strcat(reply,playername[i].c_str());
			strcat(reply," ");}
			SSL_write(ssl, reply, strlen(reply));
			printf("players info sent\n");
			// sending difficulty
			ss << obj;
			temp_buf = ss.str();
			sprintf(reply, temp_buf.c_str(), buf);
			SSL_write(ssl, reply, strlen(reply));
			printf("difficulty sent\n");
			// sending maze
			ifstream fs;
			fs.open("file.txt");
			fs.seekg(0,ios::beg);
			bzero(reply, 10000);
			fs.read(reply,10000);
			SSL_write(ssl, reply, strlen(reply));  
			printf("maze info sent\n");
			fs.close();
			send_count++;
			printf("send_count=%d\n",send_count);
			printf("players=%d\n",players);
			if (send_count==(players-1)) {printf("flag set to 1\n");flag=1;}
    }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}

void *func(void *k){
	SSL *ssl;
	ssl = (SSL *) k;
	Servlet(ssl);
}


std::string exec2(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}


 
int server_conn()
{   SSL_CTX *ctx;
    
    strcpy(ip,exec2("/sbin/ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'").c_str());
	ipaddress[0]=ip;
	ipaddress[1]=ip;
	ipaddress[2]=ip;
	ipaddress[3]=ip;
	ipaddress[4]=ip;
	ipaddress[5]=ip;
	printf("Server ip : %s \n",ip);
    SSL_library_init();
	my_id=1;
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, "mycert.pem", "mycert.pem"); /* load certs */
    server = OpenListener(20000);    /* create server socket */
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    while (players<6 and (!send_difficulty))
    {
		printf("entered loop\n");
		if (send_difficulty) { break;}
		int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
        TOTAL_PLAYERS++;
        if (send_difficulty) { break;} 
        printf("client accepted\n");
        temp_ssl = SSL_new(ctx);           /* get new SSL state with context */
        SSL_set_fd(temp_ssl, client);      /* set connection socket to SSL state */
        pthread_t t1;
        if (send_difficulty) { break;}
        pthread_create(&t1,NULL,func,(void *) temp_ssl);
        printf("thread created");
        if (send_difficulty) { break;}
    }
    while (send_count!=(players-1)) {};
    close(server);     /* close server socket */
    SSL_CTX_free(ctx);         /* release context */
}


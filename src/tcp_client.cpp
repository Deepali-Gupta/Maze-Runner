#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sstream>
#include <fstream>

#include "main.h"
#include "buttons.h"
#include "display.h"
#include "tcp_server.h"
#include "tcp_client.h"


using namespace std;
 
#define FAIL    -1

char ip_client[10000];
int flag_client=0;

int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}
 
SSL_CTX* InitCTX(void)
{   const SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = SSLv3_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

std::string exec(char* cmd) {
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
 
void set_players(){
		ifstream ifs;
		ifs.open("players.txt");
		for (int i=0;i<6;i++) {
			ifs>>ipaddress[i];
			ifs>>playername[i];
			}
		ifs.close();
}
 
int client_conn(char * server_ip)
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    int bytes;
    char hostname[10000];
	const char* server_ip1 = (const char*)server_ip;
	strcpy(ip_client,exec("/sbin/ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'").c_str());
	
    SSL_library_init();
    strcpy(hostname,server_ip1);
 
    ctx = InitCTX();
    server = OpenConnection(hostname, 20000);
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {   
		char msg[10000];
		char buf[10000];
		strcpy(msg,ip_client);
		strcat(msg," ");
		strcat(msg,username.c_str());
		strcat(msg," ");
        SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
        
        bytes=SSL_read(ssl, buf, 10000); /* get reply & decrypt */
        buf[bytes]=0;
        printf("Received player id: %s \n", buf);
        my_id =atoi(buf);
        
        bytes=SSL_read(ssl, buf, 10000);
        buf[bytes]=0;
        TOTAL_PLAYERS = atoi(buf);
        printf("Received total players: %d \n", TOTAL_PLAYERS);
        bytes=SSL_read(ssl, buf, 10000); /* get reply & decrypt */
        buf[bytes]=0;
        printf("Received all players IP and name: %s \n", buf);
        ofstream fout;
		fout.open("players.txt");
		fout.write(buf,bytes);
		fout.close();
		set_players();
        bytes=SSL_read(ssl, buf, 10000);
        buf[bytes]=0;
        obj = atoi(buf);
        printf("Received difficulty: %d \n", obj);
        bytes=SSL_read(ssl, buf, 10000);
        buf[bytes]=0;
		fout.open("file.txt");
		fout.write(buf,bytes);
		fout.close();
        displayed=2;
        flag_client=1;
		printf("\nGame Started\n");
		stringstream ss;
		ss << "./game "<<my_id<<" "<<TOTAL_PLAYERS;
		printf("\nGame Started\n");
		system("g++ -w -o game openmaze.cpp udp_server.cpp udp_client.cpp -lglut -lGL -lGLU -lGLEW -lpthread");
		system(ss.str().c_str());
        SSL_free(ssl);        /* release connection state */
    }
       
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}



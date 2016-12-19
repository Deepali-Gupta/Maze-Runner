
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>
#include <iostream>
#include "openmaze.h"
#include "udp_server.h"
using namespace std;

#define BUFLEN 10000
int udps_fd[6],slen[6];
struct sockaddr_in udps_myaddr[6], udps_remaddr[6];
char udps_buf[6][BUFLEN];	/* message buffer */
//~ int send_ports[6]={(my_id+1)*10000,(my_id+1)*10000,(my_id+1)*10000,(my_id+1)*10000,(my_id+1)*10000,(my_id+1)*10000};
int send_ports[6]={20000,30000,40000,50000,60000,70000};

void set_write_ports(){
		send_ports[0]= 20000+my_id;
		send_ports[1]= 30000+my_id;
		send_ports[2]= 40000+my_id;
		send_ports[3]= 50000+my_id;
		send_ports[4]= 60000+my_id;
		send_ports[5]= 70000+my_id;
}

void open_write_port(int player){
	slen[player]=sizeof(udps_remaddr[player]);
	/* create a socket */
	while ((udps_fd[player]=socket(AF_INET, SOCK_DGRAM, 0))<0)
		{ printf("socket not created\n"); }
	/* bind it to all local addresses and pick any port number */
	memset((char *)&(udps_myaddr[player]), 0, sizeof(udps_myaddr[player]));
	udps_myaddr[player].sin_family = AF_INET;
	udps_myaddr[player].sin_addr.s_addr = htonl(INADDR_ANY);
	udps_myaddr[player].sin_port = htons(0);

	if (bind(udps_fd[player], (struct sockaddr *)&(udps_myaddr[player]), sizeof(udps_myaddr[player])) < 0) {
		perror("bind failed");
	}       

	/* now define remaddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */

	memset((char *) &(udps_remaddr[player]), 0, sizeof(udps_remaddr[player]));
	udps_remaddr[player].sin_family = AF_INET;
	udps_remaddr[player].sin_port = htons(send_ports[player]);
	if (inet_aton(ipaddress[player].c_str(), &(udps_remaddr[player].sin_addr))==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
	}

void udp_write(char *msg, int player)
{
	
		
		int recvlen;	/* # bytes in acknowledgement message */
		//printf("Sending packet to %s port %d\n", server, port);
		
		sprintf(udps_buf[player],"%s", msg);
		if (sendto(udps_fd[player], udps_buf[player], strlen(udps_buf[player]), 0, (struct sockaddr *)&(udps_remaddr[player]), slen[player])==-1) {
			perror("sendto");
			exit(1);
		}
	
}

void close_write_port(int player){
	close(udps_fd[player]);
	}

void open_all_write_ports(){
	for (int i=0;i<6;i++) {
		if (i!=my_id) open_write_port(i);
		}
}

void send_to_all(float request,float x, float y){
		char msg[1000];
		char buf[1000];
		sprintf(buf,"%f",request);
	    strcpy(msg,buf);
		strcat(msg," ");
		sprintf(buf,"%f",x);
		//strcpy(msg,buf);
		strcat(msg,buf);
		strcat(msg," ");
		sprintf(buf,"%f",y);
		strcat(msg,buf);
		for (int i=0;i<6;i++){
			if (i!=my_id) udp_write(msg,i);
			//~ udp_write(msg,1);
			}
}



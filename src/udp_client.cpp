
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "openmaze.h"
#include "udp_client.h"

// player is between 0 and 5

#define BUFSIZE 10000
	struct sockaddr_in myaddr[6];	/* our address */
	struct sockaddr_in remaddr[6];	/* remote address */
	socklen_t addrlen = sizeof(remaddr[0]);		/* length of addresses */
	int recvlen[6];			/* # bytes received */
	int udpc_fd[6];				/* our socket */
	char udpc_buf[6][BUFSIZE];	/* receive buffer */
	int rec_ports[6] = {20000,20001,20002,20003,20004,20005};

void set_read_ports(){
		rec_ports[0]= 20000+(my_id*10000);
		rec_ports[1]= 20001+(my_id*10000);
		rec_ports[2]= 20002+(my_id*10000);
		rec_ports[3]= 20003+(my_id*10000);
		rec_ports[4]= 20004+(my_id*10000);
		rec_ports[5]= 20005+(my_id*10000);
}

void open_read_port(int player){
	/* create a UDP socket */
	while ((udpc_fd[player] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
	}
	/* bind the socket to any valid IP address and a specific port */
	memset((char *)&(myaddr[player]), 0, sizeof(myaddr[player]));
	myaddr[player].sin_family = AF_INET;
	myaddr[player].sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr[player].sin_port = htons(rec_ports[player]);

	if (bind(udpc_fd[player], (struct sockaddr *)&(myaddr[player]), sizeof(myaddr[player])) < 0) {
		perror("bind failed");
	}
	}

void udp_read(int player)
{
		//printf("waiting on port %d\n", rec_ports[player]);
		//~ std::string sss= "" ;
		recvlen[player] = recvfrom(udpc_fd[player], udpc_buf[player], BUFSIZE, 0, (struct sockaddr *)&(remaddr[player]), &addrlen);
		if (recvlen[player] > 0) {
			udpc_buf[player][recvlen[player]] = 0;
			//~ sss= udpc_buf[player];
			/*std::ofstream file;
			file.open("data.txt");
			file.write(udpc_buf[player],1000);
			file.close();
			std::ifstream file2;
			file2.open("data.txt");
			char pch[1000];
			//strcpy(pch,udpc_buf[player]);
			
			int rq;
			file2>>pch;
			rq=atoi(pch);
			if (rq==1)
			{file2>>pch;
			x_at[player]=atof(pch);
			file2>>pch;
			y_at[player]=atof(pch);}
			if (rq==2)
			{
				is_shoot[player]=1;
				file2>>pch;
				rot_player[player]=atof(pch);
				}
			if (rq==3)
			{
				file2>>pch;
				int a = atoi(pch);
				file2>>pch;
				int b = atoi(pch);
				flag[a][b]=1;
			}*/
			
			int rq;
			std::stringstream ss;
			ss << udpc_buf[player];
			char temp[100];
			ss >> temp;
			rq=atoi(temp);
			if (rq==1)
			{
				ss >> temp;
				x_at[player]=atof(temp);
				ss >> temp;
				y_at[player]=atof(temp);
			}
			if (rq==2)
			{
				is_shoot[player]=1;
				ss >> temp;
				rot_player[player]=atof(temp);
			}
			if (rq==3)
			{
				ss >> temp;
				int a = atoi(temp);
				ss>>temp;
				int b = atoi(temp);
				flag[a][b]=1;
			}
			if (rq==4)
			{
				ss >> temp;
				int a = atoi(temp);
				ss>>temp;
				int b = atoi(temp);
				flag2[a][b]=1;
			}
			if (rq==5)
			{
				ss >> temp;
				score[player]=atoi(temp);
			}
			
			
			
			
			/*pch = strtok(udpc_buf[player]," ");
			rq=atof(pch);
			pch = strtok(NULL," ");
			x_at[player]=atof(pch);
			pch = strtok(NULL," ");
			y_at[player] = atof(pch);*/
			//pch = strtok(NULL," ");
		}
		//~ else
			//~ sss= "";
		//~ return sss;
}

void close_read_port(int player){
	close(udpc_fd[player]);
	}

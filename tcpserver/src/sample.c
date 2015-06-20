#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "tcpserver.h"
#include "options.h"

void version(char * cmd) {
	printf("TCP Server version %s\n",MDE_TCPSERVER_VERSION);
	printf("https://github.com/marc-despland/samples\n\n");
}


int handler(int clientfd, struct sockaddr_in client, socklen_t clientsize) {
	int go=1;
	int n;
	char buffer[100];
	char address[INET6_ADDRSTRLEN];


	inet_ntop(AF_INET, &(client.sin_addr), address, INET6_ADDRSTRLEN );
	printf("Coonection established from %s\n",address);
	send(clientfd,"You are connected to a test server\n", 35, 0);
	send(clientfd,"Send CTRL+D to end the connection, the server will echo all other input\n\n", 71, 0);
	while (go) {
		n = recv(clientfd,buffer,sizeof(buffer)-2,0);
		//printf("Receive %d char\n",n);
		if (n>0) {
			buffer[n]=0;
			if (strchr(buffer, 4)!=NULL){
				printf("Receive quit\n");
				go=0;
			} else {
				//sprintf(buffer,"%s\r\n",buffer);
				send(clientfd,buffer,strlen(buffer),0);
			}
		} else {
			printf("Client has closed the connection\n");
			go=0;
		}
	}
	return 1;
}

int main(int argc, char **argv) {
	unsigned int port=0;
	unsigned int pool=12;
	Option options[]={
			{'n',"poolsize","The number of simultaneous clients that can connect", TRUE,FALSE,FALSE,&pool, INT},
			{'p',"port","The port to listen to", TRUE,TRUE,FALSE,&port, INT}
	};
	int result= parse_options(argc, argv, options, 2, version);
	if (result<0) {
		return 0;
	} else {
		listen_request(port,pool,&handler);
    	return 1;
    }
}

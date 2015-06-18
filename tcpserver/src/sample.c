#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "tcpserver.h"

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

int main() {

	listen_request(6666,12,&handler);
    return 1;
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcpclient.h"


int client_connect(char * host, unsigned int port, client_handler handler, int input, int output) {
	int serverfd;
	struct sockaddr_in server;
	long hostaddress;
	struct hostent *serverent;
	
	tcperrno=0;
	bzero(&server,sizeof(server));
	hostaddress = inet_addr(host);
	if ( (long)hostaddress != (long)-1) {
		bcopy(&hostaddress,&server.sin_addr,sizeof(hostaddress));
	} else {
		serverent = gethostbyname(host);
		if (serverent == NULL){
			tcperrno=errno;
			return ERRINVALIDHOSTNAME;
		}
		bcopy(serverent->h_addr,&server.sin_addr,serverent->h_length);
	}
	server.sin_port = htons(port);
	server.sin_family = AF_INET;

	if ( (serverfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		tcperrno=errno;
		return ERRCANTCREATESOCKET;
	}
	if (connect( serverfd,(struct sockaddr *)&server, sizeof(server)) < 0 ) {
		tcperrno=errno;
		return ERRCANTCONNECTSOCKET;
	}
	
	handler(serverfd, input, output);
	
	shutdown(serverfd,2);
	close(serverfd);
	return 1;
}

#include "tcpclient.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

TcpClient::TcpClient():Client() {
}

TcpClient::~TcpClient() {
}

void TcpClient::start(char * host, int port)  throw(CantConnectException) {
	this->server=strdup(host);
	this->port=port;
	
	struct sockaddr_in server;
	long hostaddress;
	struct hostent *serverent;
	
	bzero(&server,sizeof(server));
	hostaddress = inet_addr(this->server);
	if ( (long)hostaddress != (long)-1) {
		bcopy(&hostaddress,&server.sin_addr,sizeof(hostaddress));
	} else {
		serverent = gethostbyname(this->server);
		if (serverent == NULL){
			throw CantConnectException(errno);
		}
		bcopy(serverent->h_addr,&server.sin_addr,serverent->h_length);
	}
	server.sin_port = htons(this->port);
	server.sin_family = AF_INET;

	if ( (this->serverfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		throw CantConnectException(errno);
	}
	if (connect( this->serverfd,(struct sockaddr *)&server, sizeof(server)) < 0 ) {
		throw CantConnectException(errno);
	}
	this->setEncodedFd(this->serverfd, this->serverfd);
	this->run();
}

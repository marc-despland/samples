#include "tcpconnection.h"


TcpConnection::~TcpConnection() {
}

TcpConnection::TcpConnection(TcpServerAction * server,TcpConnectionListener * listener):Fork() {
	this->server=server;
	this->listener=listener;
}


void TcpConnection::connected(int clientfd, struct sockaddr_in client,socklen_t clientsize)  throw (ForkException){
	this->clientfd=clientfd;
	this->client=client;
	this->clientsize=clientsize;
	this->execute();
}

void TcpConnection::parent() {
	//Do nothing
}

void TcpConnection::child() {
	this->server->connected(this->clientfd, this->client, this->clientsize);
	this->listener->connectionClosed(this->clientfd);
	
}


#include "tcpconnection.h"
#include <iostream>
#include "log.h"

TcpConnection::~TcpConnection() {
}

TcpConnection::TcpConnection(TcpServerAction * server,TcpConnectionListener * listener):Runnable(), Fork(this) {
	this->server=server;
	this->listener=listener;
}


void TcpConnection::connected(int clientfd, struct sockaddr_in client,socklen_t clientsize)  throw (ForkException){
	Log::logger->log("TCP", DEBUG) << "TcpConnection connected" << endl;
	this->clientfd=clientfd;
	this->client=client;
	this->clientsize=clientsize;
	this->execute();
}

void TcpConnection::parent() {
	//Do nothing
}

void TcpConnection::child() {
	this->server->connected(this->status, this->clientfd, this->client, this->clientsize);
	this->listener->connectionClosed(this->clientfd);
	
}


void TcpConnection::halt() {
	Log::logger->log("TCP", DEBUG) << "Halt the TcpConnection" << endl;
	this->stop();
	this->killChild();
}

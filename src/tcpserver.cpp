#include "tcpserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "log.h"

extern "C" int listen(int sockfd, int backlog);

TcpServer::TcpServer(TcpServerAction * action):Runnable() {
	this->action=action;
}
TcpServer::~TcpServer() {

}

void TcpServer::connectionClosed(int socket) {
	Log::logger->log("TCP", DEBUG) << "Received ConnectionClosed for socket " << socket << endl;
	TcpConnection * connection=this->pool[socket];
	if (connection!=NULL) {
		close(socket);
		delete connection;
	}
}

void TcpServer::stop() {
	Runnable::stop();
	//we have to close all client socket
	for (std::map<int, TcpConnection *>::iterator it=this->pool.begin(); it!=this->pool.end(); ++it) {
		it->second->halt();
	}
	close(this->listenfd);
}

void TcpServer::start(unsigned int port, unsigned int pool_size) throw(TcpServerBindException) {
	int clientfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t clientsize;

	this->listenfd=socket(AF_INET,SOCK_STREAM,0);

	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(port);
	if (bind(this->listenfd,(struct sockaddr *)&server,sizeof(server))<0) {
		 throw TcpServerBindException();
	}

	if (listen(this->listenfd,pool_size)<0) {
		 throw TcpServerBindException();
	}
	this->Runnable::start();
	while (this->running()) {
		bzero(&client,sizeof(client));
		clientsize=sizeof(client);
		clientfd = accept(this->listenfd,(struct sockaddr *)&client,&clientsize);
		if (clientfd>0) {
			Log::logger->log("TCP", DEBUG) << "Received a connection on listen socket " << clientfd<< endl;
			TcpConnection * connection=new TcpConnection(this->action, this);
			try {
				connection->connected(clientfd, client, clientsize);
				this->pool[clientfd]=connection;
			} catch (ForkException &e) {
				Log::logger->log("TCP", CRITICAL) << "Can't fork to create TcpConnection " << endl;
			}
		}
	}
}

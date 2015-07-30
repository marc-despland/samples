#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H
#include <sys/socket.h>
#include <netinet/in.h>
#include "fork.h"
#include "tcpserveraction.h"
#include "tcpconnectionlistener.h"
#include "runnable.h"

class TcpConnection : public Runnable, public Fork {
	public :
		TcpConnection(TcpServerAction * server, TcpConnectionListener * listener);
		virtual ~TcpConnection();
		void connected(int clientfd, struct sockaddr_in client,socklen_t clientsize) throw (ForkException);
		void halt();
	protected : 
		int clientfd;
		struct sockaddr_in client;
		socklen_t clientsize;
		TcpServerAction * server;
		TcpConnectionListener * listener;
		void parent();
		void child();
};
#endif

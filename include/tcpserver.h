#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include "tcpconnection.h"
#include "tcpconnectionlistener.h"
#include "runnable.h"
#include <map>

class TcpServer: public virtual Runnable, public TcpConnectionListener{
	public:
		TcpServer(TcpServerAction * action);
		void start(unsigned int port, unsigned int pool_size);
		void connectionClosed(int socket);
		void stop();
		
	protected:
		TcpServerAction * action;
		map<int, TcpConnection *>	pool;
		int listenfd;


};

#endif

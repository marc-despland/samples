#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include "tcpconnection.h"
#include "tcpconnectionlistener.h"
#include "runnable.h"
#include <map>

class TcpServerBindException : public exception {
	const char* what();
};

class TcpServer: public virtual Runnable, public TcpConnectionListener{
	public:
		TcpServer(TcpServerAction * action);
		virtual ~TcpServer();
		void start(unsigned int port, unsigned int pool_size) throw(TcpServerBindException);
		void connectionClosed(int socket);
		void stop();
		
	protected:
		TcpServerAction * action;
		map<int, TcpConnection *>	pool;
		int listenfd;


};

#endif

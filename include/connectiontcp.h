#ifndef _CONNECTIONTCP_H
#define _CONNECTIONTCP_H
#ifdef HAVE_TCP_CONNECTION
#include "connection.h"

class ConnectionTCP:public Connection {
	public:
		ConnectionTCP(Host * host);
		ConnectionTCP(int socket,Host * host);
		virtual ~ConnectionTCP();
		
		virtual void listen(unsigned int size) throw(ConnectionListenException);
		virtual ConnectionTCP * accept() throw(ConnectionAcceptException);
		virtual void shutdown();
		virtual void connect() throw(CantConnectException);
};

#endif
#endif
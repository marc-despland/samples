#ifndef _SERVER_H
#define _SERVER_H

#include <string>
#include <exception>
#include <map>
using namespace std;
#include "runnable.h"
#include "clienthandler.h"
#include "connection.h"
#include "host.h"
#include "clienthandlerfactory.h"
#ifdef HAVE_TCP_CONNECTION
#include "connectiontcp.h"
#endif


template <typename Cnx>   
class Server: public Runnable, public ConnectionListener{
	public:
		virtual ~Server();
		Server(Host * endpoint,unsigned int size, ClientHandlerFactory * factory);
		unsigned int poolSize();
		unsigned int poolFree();
		void start() throw(ConnectionListenException);
		void stop();
		void established(Connection * cnx);
		void closed(Connection * cnx);

	protected:
		unsigned int size;
		map<int, ClientHandler *> pool;
		Host * endpoint;
		Cnx * connection;
		ClientHandlerFactory * factory;
};
#endif
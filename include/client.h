#ifndef _CLIENT_H
#define _CLIENT_H
using namespace std;
#include "runnable.h"
#include "connection.h"
#include "host.h"
#ifdef HAVE_TCP_CONNECTION
#include "connectiontcp.h"
#endif
#include "host.h"



template <typename Cnx>   
class Client: virtual public Runnable, public ConnectionListener {
	public:
		Client();
		virtual ~Client();
		virtual void close();

		void connect(Host * server) throw(CantConnectException);
		void established(Connection * cnx);
		void closed(Connection * cnx);
	protected:
	
		Cnx * cnx;
		virtual void execute()=0;
};
#endif

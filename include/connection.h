#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <string>
#include "connectionlistener.h"
#include "host.h"
using namespace std;

class ConnectionListenException: public exception {
	public:
		ConnectionListenException(string message):exception() {
			this->message=message;
		}
		~ConnectionListenException() throw (){};
		friend std::ostream & operator<<(std::ostream &os, const ConnectionListenException& e) {
			return os << e.message;
		}
	protected:
		string message;
};
class ConnectionAcceptException: public exception {
	public:
		ConnectionAcceptException(string message):exception() {
			this->message=message;
		}
		~ConnectionAcceptException() throw (){};
		friend std::ostream & operator<<(std::ostream &os, const ConnectionAcceptException& e) {
			return os << e.message;
		}
	protected:
		string message;
};


class Connection {
	public:
		Connection();
		Connection(Host * host);
		Connection(int socket,Host * host);
		virtual ~Connection();
		int id();
		bool isConnected();
		int socket();
		void registerListener(ConnectionListener * listener);
		Host * endpoint();
		
		
		virtual void listen(unsigned int size) throw(ConnectionListenException);
		virtual Connection * accept() throw(ConnectionAcceptException);
		virtual void shutdown();
		
		friend std::ostream & operator<<(std::ostream &os, const Connection * c);
		
	protected:
		int 					socketfd;
		bool 					isconnected;
		ConnectionListener * 	listener;
		Host * 					host;
};

#endif
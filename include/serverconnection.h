#ifndef _SERVERCONNECTION_H
#define _SERVERCONNECTION_H
#include "connection.h"
#include <exception>

using namespace std;


class ServerConnectionException: public exception {
	public:
		ServerConnectionException(string message);
		const char* what();
	protected:
		string message;
};
class ServerAcceptException: public exception {
	public:
		ServerAcceptException(string message);
		const char* what();
	protected:
		string message;
};


class ServerConnection: public Connection {
	public:
		ServerConnection(ServerParameters * params) throw (ServerConnectionException);
		virtual ~ServerConnection();
		virtual Connection * accept()=0 throw (ServerAcceptException);
		virtual void listen()=0 throw (ConnectionListenException);
				
	protected:
		ServerParameters * params;


};
#endif
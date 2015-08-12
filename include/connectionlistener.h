#ifndef _CONNECTIONLISTENER_H
#define _CONNECTIONLISTENER_H
#include "connection.h"
class Connection;

class ConnectionListener {
	public:
		virtual void established(Connection * cnx)=0;
		virtual void closed(Connection * cnx)=0;
};

#endif
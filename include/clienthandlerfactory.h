#ifndef _CLIENTHANDLERFACTORY_H
#define _CLIENTHANDLERFACTORY_H

#include "clienthandler.h"
#include "connection.h"

class ClientHandlerFactory {
	public:
		virtual ClientHandler * createClientHandler(Connection * cnx)=0;

};

#endif
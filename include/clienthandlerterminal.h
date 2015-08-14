#ifndef _CLIENTHANDLERTERMINAL_H
#define _CLIENTHANDLERTERMINAL_H

#include "clienthandler.h"
#include "clienthandlerfactory.h"
#include "authentication.h"
#include "termtty.h"

class ClientHandlerTerminal:public ClientHandler {
	public:	
		ClientHandlerTerminal(Connection *cnx);
		~ClientHandlerTerminal();
		void action() ;
		void terminate();
	protected:
		TermTTY * terminal;
};

class TerminalFactory: public ClientHandlerFactory {
	public:
		ClientHandlerTerminal * createClientHandler(Connection * cnx) ;
};



#endif
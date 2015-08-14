#ifndef _CLIENTHANDLER_H
#define _CLIENTHANDLER_H
#include "fork.h"
#include "connection.h"

class ClientHandler:public Fork {
	public:
		ClientHandler(Connection * cnx);
		virtual ~ClientHandler();
		void handle() throw (ForkException);
		virtual void interrupt();
		virtual void action() =0;
		void child();
		void parent();
		
	protected:
		Connection * cnx;
		void terminated();

};


#endif
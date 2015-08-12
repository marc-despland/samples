#ifndef _CLIENTHANDLER_H
#define _CLIENTHANDLER_H
#include "fork.h"
#include "connection.h"
#include "runnable.h"

class ClientHandler:public Runnable,public Fork {
	public:
		ClientHandler(Connection * cnx);
		virtual ~ClientHandler();
		void start() throw (ForkException);
		virtual void stop();
		virtual void action() =0;
		void child();
		void parent();
		
	protected:
		Connection * cnx;
		void terminated();

};


#endif
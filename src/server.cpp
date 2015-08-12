#include "server.h"
#include "log.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
template <typename Cnx>   

Server<Cnx>::~Server() {
}

template <typename Cnx>   
Server<Cnx>::Server(Host * endpoint,unsigned int size, ClientHandlerFactory * factory): Runnable()  {
	this->size=size;
	this->endpoint=endpoint;
	this->connection=new Cnx(this->endpoint);
	this->factory=factory;
}


template <typename Cnx>   
unsigned int Server<Cnx>::poolSize() {
	return this->size;
}

template <typename Cnx>   
unsigned int Server<Cnx>::poolFree() {
	return this->size-this->pool.size();
}

template <typename Cnx>   
void Server<Cnx>::start() throw(ConnectionListenException) {
	this->connection->listen(this->size);
	int error=0;
	this->Runnable::start();
	while (this->Runnable::running()) {
		try {
			Cnx * client=this->connection->accept();
			Log::logger->log("SERVER", DEBUG) << "New connection established " << client->endpoint() << endl;
			client->registerListener(this);
			Log::logger->log("SERVER", DEBUG) << "Listener registered " << client->endpoint() << endl;
			ClientHandler * handler=this->factory->createClientHandler(client);
			Log::logger->log("SERVER", DEBUG) << "Handler created " << client->endpoint() << " Socket : " << client->socket()<< "Handler " << handler<<endl;
			this->pool[client->socket()]=handler;
			Log::logger->log("SERVER", DEBUG) << "Handler add in pool " << client->endpoint() << endl;
			try {
				Log::logger->log("SERVER", DEBUG) << "Starting Handler " << client->endpoint() << endl;
				handler->handle();
				Log::logger->log("SERVER", DEBUG) << "Handler start " << client->endpoint() << endl;
				
			} catch (ForkException &e) {
				Log::logger->log("SERVER", CRITICAL) << "Can't start handler : closing client connection" << endl;
				client->shutdown();
			}
			
		} catch(ConnectionAcceptException &e) {
			Log::logger->log("SERVER",ERROR) << "Failed to accept new clients, we stop the server : " << e <<endl;
			this->Runnable::stop();
		}
	}
}


template <typename Cnx>   
void Server<Cnx>::stop() {
	this->Runnable::stop();
	//we have to close all client socket
	for (map<int, ClientHandler *>::iterator it=this->pool.begin(); it!=this->pool.end(); ++it) {
		it->second->interrupt();
	}
	this->connection->shutdown();
}



template <typename Cnx>   
void Server<Cnx>::established(Connection * cnx) {
	Log::logger->log("SERVER",DEBUG) << "Connection established " << cnx <<endl;
			
}

template <typename Cnx>   
void Server<Cnx>::closed(Connection * cnx) {
	Log::logger->log("SERVER",DEBUG) << "Connection closed " << cnx <<endl;
	if (this->pool.count(cnx->socket())==1) {
		ClientHandler * handler=this->pool[cnx->socket()];
		this->pool.erase(cnx->socket());
		delete handler;
		delete cnx;
	}
}





template class Server<Connection>;

#ifdef HAVE_TCP_CONNECTION
template class Server<ConnectionTCP>;
#endif

#ifdef HAVE_RFCOMM_CONNECTION
template class Server<ConnectionRFCOMM>;
#endif

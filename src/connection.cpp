#include "connection.h"
#include "log.h"

Connection::Connection() {
	this->isconnected=false;
	this->socketfd=-1;
	this->listener=NULL;
	this->host=NULL;
}

Connection::Connection(Host * host) {
	this->isconnected=false;
	this->socketfd=-1;
	this->listener=NULL;
	this->host=host;
}

Connection::Connection(int socket,Host * host) {
	this->isconnected=false;
	this->socketfd=socket;
	this->listener=NULL;
	this->host=host;
}

int Connection::socket() {
	return this->socketfd;
}

bool Connection::isConnected() {
	return this->isconnected;
}

Connection::~Connection() {
}

void Connection::listen(unsigned int size) throw(ConnectionListenException){
	this->isconnected=true;
	if (this->listener!=NULL) {
		this->listener->established(this);
	}
}

void Connection::registerListener(ConnectionListener * listener) {
	this->listener=listener;
}

void Connection::shutdown() {
	if (this->listener!=NULL) {
		this->listener->closed(this);
	}
}

Connection * Connection::accept() throw(ConnectionAcceptException) {
	throw ConnectionAcceptException("Operation not implemented on generic Connection class");
}


void Connection::connect() throw(CantConnectException) {
	this->isconnected=true;
	if (this->listener!=NULL) {
		this->listener->established(this);
	}
}


Host * Connection::endpoint() {
	return this->host;
}

std::ostream & operator<<(std::ostream &os, const Connection * c) {
	return os << c->host;
}
		
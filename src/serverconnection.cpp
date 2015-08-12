#include "serverconnection.h"

ServerConnectionException::ServerConnectionException(string message):exception() {
	this->message=message;
}

const char* ServerConnectionException::what() {
	return this->message.c_str();
}
ServerAcceptException::ServerAcceptException(string message):exception() {
	this->message=message;
}

const char* ServerAcceptException::what() {
	return this->message.c_str();
}




ServerConnection::ServerConnection(unsigned int pool):Connection() {
	this->poolsize=pool;
}

ServerConnection::~ServerConnection() {
}

unsigned int ServerConnection::pool() {
	return this->poolsize;
}
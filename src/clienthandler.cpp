#include "clienthandler.h"
#include "log.h"

ClientHandler::ClientHandler(Connection * cnx):Runnable(),Fork(this,"ClientHandler") {
	this->cnx=cnx;
}

ClientHandler::~ClientHandler() {
}

void ClientHandler::handle() throw (ForkException) {
	Log::logger->log("CLTHLD",DEBUG) << "Starting clienthandler" << endl;
	this->execute();
}

void ClientHandler::parent() {
	Log::logger->log("CLTHLD",DEBUG) << "Executing PARENT from clienthandler" << endl;
	//Do nothing
}

void ClientHandler::child() {
	Log::logger->log("CLTHLD",DEBUG) << "Executing CHILD from clienthandler" << endl;
	//this->Runnable::start();
	this->action();
	this->cnx->shutdown();
	
}

void ClientHandler::terminated() {
	this->cnx->shutdown();
}

void ClientHandler::interrupt() {
	Log::logger->log("CLTHLD",DEBUG) << "Stop the client handler" << endl;
	this->killChild();
}

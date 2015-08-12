#include "clienthandler.h"
#include "log.h"

ClientHandler::ClientHandler(Connection * cnx):Runnable(),Fork(this,"ClientHandler") {
	this->cnx=cnx;
}

ClientHandler::~ClientHandler() {
}

void ClientHandler::start() throw (ForkException) {
	this->execute();
}

void ClientHandler::parent() {
	//Do nothing
}

void ClientHandler::child() {
	Log::logger->log("CLTHLD",DEBUG) << "Executing CHILD from clienthandler" << endl;
	this->Runnable::start();
	this->action();
	this->cnx->shutdown();
	
}

void ClientHandler::terminated() {
	this->cnx->shutdown();
}

void ClientHandler::stop() {
	Log::logger->log("CLTHLD",DEBUG) << "Stop the client handler" << endl;
	this->killChild();
}

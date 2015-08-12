#include "clienthandlerterminal.h"
#include "log.h"

ClientHandlerTerminal::ClientHandlerTerminal(Connection *cnx):ClientHandler(cnx) {
}

void ClientHandlerTerminal::action() {
	Log::logger->log("CLHLDT", NOTICE) << "Connection established from "<< this->cnx->endpoint() <<endl;
	Log::logger->log("CLHLDT", DEBUG) << "Start authentification" <<endl;
	Authentication * auth=new Authentication();
	if (auth->authenticate(this->cnx->socket(),this->cnx->socket())) {
		Log::logger->log("CLHLDT", DEBUG) << "Client authenticate : uid =" << auth->uid() <<endl;
		TermTTY * terminal=new TermTTY(this, this->cnx->socket(),this->cnx->socket());
		terminal->setUser(auth->uid(), auth->gid());
		terminal->terminal();
		delete terminal;
	} else {
		Command quit(Command::QUIT,"");
		quit.send(this->cnx->socket());
	}
	Log::logger->log("CLHLDT", NOTICE) << "Disconnecting from "<<this->cnx->endpoint()<<endl;
	delete auth;
}


ClientHandlerTerminal * TerminalFactory::createClientHandler(Connection * cnx) {
	return new ClientHandlerTerminal(cnx);
}

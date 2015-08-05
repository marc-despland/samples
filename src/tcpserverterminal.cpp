#include "tcpserverterminal.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"
#include "authentication.h"

TcpServerTerminal::~TcpServerTerminal() {

}

TcpServerTerminal::TcpServerTerminal(): TcpServer(this) {

}

void TcpServerTerminal::connected(IRunnable * status,int clientfd, struct sockaddr_in client,socklen_t clientsize) {
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client.sin_addr), address, INET6_ADDRSTRLEN );
	Log::logger->log("TCPTERM", NOTICE) << "Connection established from "<<address <<endl;
	Log::logger->log("TCPTERM", DEBUG) << "Start authentification" <<endl;
	Authentication * auth=new Authentication();
	if (auth->authenticate(clientfd, clientfd)) {
		Log::logger->log("TCPTERM", DEBUG) << "Client authenticate : uid =" << auth->uid() <<endl;
		TermTTY * terminal=new TermTTY(status, clientfd, clientfd);
		terminal->setUser(auth->uid(), auth->gid());
		terminal->terminal();
		delete terminal;
	} else {
		Command quit(Command::QUIT,"");
		quit.send(clientfd);
	}
	Log::logger->log("TCPTERM", NOTICE) << "Disconnecting from "<<address<<endl;
	delete auth;
}

#include "tcpserverterminal.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"

TcpServerTerminal::~TcpServerTerminal() {

}

TcpServerTerminal::TcpServerTerminal(): TcpServer(this) {

}

void TcpServerTerminal::connected(IRunnable * status,int clientfd, struct sockaddr_in client,socklen_t clientsize) {
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client.sin_addr), address, INET6_ADDRSTRLEN );
	Log::logger->log("GLOBAL", NOTICE) << "Connection established from "<<address <<endl;
	TermTTY * terminal=new TermTTY(status, clientfd, clientfd);
	terminal->terminal();
	Log::logger->log("GLOBAL", NOTICE) << "Disconnecting from "<<address<<endl;
	delete terminal;
}

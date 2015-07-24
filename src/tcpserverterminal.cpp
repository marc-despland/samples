#include "tcpserverterminal.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>


TcpServerTerminal::TcpServerTerminal(): TcpServer(this) {

}

void TcpServerTerminal::connected(int clientfd, struct sockaddr_in client,socklen_t clientsize) {
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client.sin_addr), address, INET6_ADDRSTRLEN );
	cout << "Connection established from "<<address << " pid: " << getpid()<<endl;
	TermTTY * terminal=new TermTTY(clientfd, clientfd);
	terminal->terminal();
	cout << "Disconnecting from "<<address<<endl;
	delete terminal;
}

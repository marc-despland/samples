#ifndef _TCPSERVERTERMINAL_H
#define _TCPSERVERTERMINAL_H
#include "tcpserver.h"
#include "termtty.h"

class TcpServerTerminal:public TcpServer, public TcpServerAction {
	public:
		TcpServerTerminal();
		virtual ~TcpServerTerminal();
		void connected(IRunnable * status,int clientfd, struct sockaddr_in client,socklen_t clientsize);




};

#endif

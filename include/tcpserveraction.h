#ifndef _TCPSERVERACTION_H
#define _TCPSERVERACTION_H
#include <sys/socket.h>
#include <netinet/in.h>

class TcpServerAction {
	public:
		virtual void connected(int clientfd, struct sockaddr_in client,socklen_t clientsize)=0;

};

#endif

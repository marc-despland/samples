#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include <sys/socket.h>

#define MDE_TCPSERVER_VERSION "1.1.0"

//The connection_handler is call in a new child to manage the client that has just
//establish a connection with the server.
typedef int (*connection_handler)(int, struct sockaddr_in , socklen_t);
int listen_request(unsigned int port, unsigned int pool_size, connection_handler handler);

#endif

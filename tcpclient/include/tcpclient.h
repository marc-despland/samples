#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include <sys/socket.h>

//Can't retrieve the IP for the given host name
#define ERRINVALIDHOSTNAME	-1
#define ERRCANTCREATESOCKET -2
#define ERRCANTCONNECTSOCKET -3

int tcperrno;

typedef int (*connection_handler)(int, struct sockaddr_in , socklen_t);
int listen_request(unsigned int port, unsigned int pool_size, connection_handler handler);

#endif

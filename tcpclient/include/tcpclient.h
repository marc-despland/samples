#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include <sys/socket.h>

//Can't retrieve the IP for the given host name
#define ERRINVALIDHOSTNAME	-1
#define ERRCANTCREATESOCKET -2
#define ERRCANTCONNECTSOCKET -3

int tcperrno;

typedef int (*client_handler)(int, int, int);
int client_connect(char * host, unsigned int port, client_handler handler, int input, int output); 
#endif

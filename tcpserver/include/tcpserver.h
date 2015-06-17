#ifndef _TCPSERVER_H
#define _TCPSERVER_H
typedef int (*connection_handler)(int);
int listen_request(unsigned int port, unsigned int pool_size, connection_handler handler);

#endif
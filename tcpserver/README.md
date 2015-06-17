# TCP Server

## Description 
TcpServer is a simple implementation of a tcp server that manage text request. 
* src/tcpserver.c : implement the function of the server
* include/tcpserver.h : the header
* src/sample.c : an exemple using tcpserver to implement a simple server

## Usage
To implement a tcp server you just have to call the function listent_request

```int listen_request(unsigned int port, unsigned int pool_size, connection_handler handler); ```

* port : the port to listen
* pool_size : the number of simumtaneous connection that can manage the server
* handler : the function that manage a connection

```int (*connection_handler)(int, struct sockaddr_in , socklen_t);```

The function that implement the management of one client
* int : the client socket fd
* struct sockaddr_in : the structure return by ``accept`` that describe the client
* socklen_t : the size return by ``accept``

## Build
``` make ```

# TCP Client

## Description 
TcpClient is a simple implementation of a tcp client that manage text request. 
* src/tcpclient.c : implement the function of the client
* include/tcpclient.h : the header
* src/sample.c : an exemple using tcpclient to implement a simple client

A simple telnet client could be use to test the sample server

## How to use
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

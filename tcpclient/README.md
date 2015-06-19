# TCP Client

## Description 
TcpClient is a simple implementation of a tcp client that manage text request. 
* src/tcpclient.c : implement the function of the client
* include/tcpclient.h : the header
* src/sample.c : an exemple using tcpclient to implement a simple client

The TcpClient is design to work with the sample of the TcpServer

## How to use
To implement a tcp server you just have to call the function client_connect

```int client_connect(char * host, unsigned int port, client_handler handler, int input, int output) ```

* host : the server fqdn or ip adress
* port : the port listened by the server
* handler : the function that manage a connection
* input : the input FD to send data to the server
* output : the output FD to display data received from the server

The function return 1 if everything is find, if a problem occurs the variable ``tcperrno`` is set with the value of ``errno`` and the function return :
* ERRINVALIDHOSTNAME	  -1 : Can't retrieve the IP from the given FQDN
* ERRCANTCREATESOCKET   -2 : Can't create the socket
* ERRCANTCONNECTSOCKET  -3 : Can't connect the socket

```typedef int (*client_handler)(int, int, int);```

The function that implement the communication with the server
* int : the socket conected to the server
* int : input FD
* int : output FD

## Build
``` make ```

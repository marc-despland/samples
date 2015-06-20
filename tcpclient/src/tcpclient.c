//version 1.0.1

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcpclient.h"

int serverfd=-1;
int inputfd=-1;
int outputfd=-1;

/*
Action on SIGWINCH to manage TTY resizing
*/
static void resizeTTY(int sig) {
    struct winsize termsize;
	ioctl(outputfd,TIOCGWINSZ,&termsize);
	ioctl(serverfd,TIOCSWINSZ,&termsize);
}




int client_connect(char * host, unsigned int port, client_handler handler, int input, int output) {
	struct sockaddr_in server;
	long hostaddress;
	struct hostent *serverent;
	
	inputfd=input;
	outputfd=output;
	
	tcperrno=0;
	bzero(&server,sizeof(server));
	hostaddress = inet_addr(host);
	if ( (long)hostaddress != (long)-1) {
		bcopy(&hostaddress,&server.sin_addr,sizeof(hostaddress));
	} else {
		serverent = gethostbyname(host);
		if (serverent == NULL){
			tcperrno=errno;
			return ERRINVALIDHOSTNAME;
		}
		bcopy(serverent->h_addr,&server.sin_addr,serverent->h_length);
	}
	server.sin_port = htons(port);
	server.sin_family = AF_INET;

	if ( (serverfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		tcperrno=errno;
		return ERRCANTCREATESOCKET;
	}
	if (connect( serverfd,(struct sockaddr *)&server, sizeof(server)) < 0 ) {
		tcperrno=errno;
		return ERRCANTCONNECTSOCKET;
	}
	
	//Create the sigaction structure to handle SIGWINCH signal
	struct sigaction eventWindowResize;
	sigemptyset(&eventWindowResize.sa_mask);
	eventWindowResize.sa_flags=0;
	eventWindowResize.sa_handler= resizeTTY;

	//Change TTY size to the actual size of the terminal
	struct winsize termsize;
	ioctl(output,TIOCGWINSZ,&termsize);
	ioctl(serverfd,TIOCSWINSZ,&termsize);

	
	handler(serverfd, input, output);
	
	shutdown(serverfd,2);
	close(serverfd);
	return 1;
}

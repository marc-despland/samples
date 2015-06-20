#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "tcpclient.h"
#include "options.h"

void version(char * cmd) {
	printf("TCP Client version %s\n",MDE_TCPCLIENT_VERSION);
	printf("https://github.com/marc-despland/samples\n\n");
}



int handler(int serverfd, int input, int output) {
	int go=1;
	char buffer[100];
	fd_set readset;
	int result;
	int maxfd;
	int count; 
    
	
	if (serverfd>input) {
		maxfd=serverfd+1;
	} else {
		maxfd=input+1;
	}
	while (go) {
		//initialize file descriptor for select
		FD_ZERO(&readset);
		FD_SET(serverfd, &readset);
		FD_SET(input, &readset);
		result = select(maxfd, &readset, NULL, NULL, NULL);
		if (result > 0) {
			if (FD_ISSET(serverfd, &readset)) {
				//Child has write on its stdout
				count = read(serverfd, buffer, sizeof(buffer)-1);
				if (count > 0) {
					buffer[count] = 0;
					write(output, buffer, count);
					//fflush(output);
				} else {
					go=0;
				}
			}
			if (FD_ISSET(input, &readset)) {
				//User has write some stuff
				count = read(input, buffer, sizeof(buffer)-1);
				if (count >= 0) {
					buffer[count] = 0;
					write(serverfd, buffer, count);
				}
			}
		} else {
			go=0;
		}
	}
	printf("End of client handler\n");
	return 1;
}

int main(int argc, char **argv) {
	char * hostname="";
	unsigned int port=0;
	int ret;
	
	Option options[]={
			{'s',"server","The server FQDN or IP to connect to", TRUE,TRUE,FALSE,&hostname, STRING},
			{'p',"port","The port to connect on the server", TRUE,TRUE,FALSE,&port, INT}
	};
	int result= parse_options(argc, argv, options, 2, version);
	if (result<0) {
		return 0;
	} else {
	
		// Backup intial TTY mode of STDIN
		struct termios orgttystate;
		tcgetattr(STDIN_FILENO, &orgttystate);

		// Update STDIN mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
		struct termios ttystate;
		tcgetattr(STDIN_FILENO, &ttystate);
		ttystate.c_lflag &= ~ICANON;
		ttystate.c_lflag &= ~ECHO;
		ttystate.c_cc[VMIN] = 1;
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
		
		printf("Trying to connect on %s port %d\n",hostname, port);
		
		ret=client_connect(hostname,port, &handler, STDIN_FILENO, STDOUT_FILENO);
	
		tcsetattr(STDIN_FILENO, TCSANOW, &orgttystate);
		printf("Client close : %d %d\n",ret,tcperrno);
		return 1;
	}
}

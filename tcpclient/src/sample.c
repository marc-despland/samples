#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "tcpclient.h"


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

int main() {
	int ret;
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

	ret=client_connect("192.168.1.23", 6666, &handler, STDIN_FILENO, STDOUT_FILENO);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &orgttystate);
	printf("Client close : %d %d\n",ret,tcperrno);
	return 1;
}

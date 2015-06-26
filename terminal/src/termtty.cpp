#include "termtty.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <pty.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <algorithm>
#include <string.h>
#include "command.h"
#include "message.h"

const char* TermTTYForkException::what() {
	return "Can't fork process to create tty";
}


TermTTY::TermTTY(int input, int output) {
	this->buffer=new Buffer();
	this->pid=-1;
	this->started=false;
	this->terminalfd=-1;
	this->input=input;
	this->output=output;
	
	struct termios ttystate;

	// Backup intial TTY mode of input fd
	tcgetattr(this->input, &(this->inputopt));

	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(this->input, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(this->input, TCSANOW, &ttystate);
}


TermTTY::~TermTTY() {
	tcsetattr(this->input, TCSANOW, &(this->inputopt));
}

bool TermTTY::execute() throw (TermTTYForkException) {
	fd_set readset;
	int result;
	sigset_t mask;
	
	
	char buffer[100];
	int count;
	int status,pid;
	
	char * message;
	
	//Define the sigmask  to catch SIGCHLD with pselect
	sigemptyset (&mask);
	sigaddset (&mask, SIGCHLD);

	//Create the fork PTY to manage our shell
	this->pid = forkpty(&(this->terminalfd), NULL, NULL, NULL);
	if (this->pid==-1) throw TermTTYForkException();
	
	if (this->pid == 0) {
		//here we are the child process
		 char *argv[]={ "/bin/bash","--login", 0};
		execv(argv[0], argv);
		//child process ends here
	} else {
	
		//Initialize the flag to signal child is alive
		this->started=true;
	
		do {
			//initialize file descriptor mask for select
			FD_ZERO(&readset);
			FD_SET(this->input, &readset);
			FD_SET(this->terminalfd, &readset);
		
		
			//Block until something to read on child stdout, parent stdin or SIGCHLD
			result = pselect(max(this->terminalfd, this->input), &readset, NULL, NULL, NULL, &mask);
			
			if (result > 0) {
				if (FD_ISSET(this->terminalfd, &readset)) {
					//Child has write on its stdout
					count = read(this->terminalfd, buffer, sizeof(buffer)-1);
					if (count >= 0) {
						buffer[count]=0;
						//message=context.encoder(buffer, &count);
						if (message!=NULL) {
							write(this->output, message, count);
							free(message);
						}
					}
				}
				if (FD_ISSET(this->input, &readset)) {
					//User has write some stuff
					count = read(this->input, buffer, sizeof(buffer)-1);
					if (count >= 0) {
						//message=context.decoder(buffer,count);
						if (message!=NULL) {
							write(this->terminalfd, message, strlen(message));
							free(message);
						}
					}
				}
			} else {
				if (errno==EINTR) {
					//Signal SIGCHLD has been caught 
					//Terminal is closed
					//wait for one child process ends
					pid=wait(&status);
					if (pid==this->pid) {
						this->started=false;
					}
				} else {
					//an error occurs, we quit 
					this->started=false;
				}
			}
		} while (this->started);
	}
	return true;
}

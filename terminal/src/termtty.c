//termtty : version 1.0.1

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
#incluse "boolean.h"
#include "termtty.h"


char * default_decoder(char * buffer, int buffersize) {
	char * msg;
	msg=(char *) malloc(sizeof(char)*(buffersize+1));
	bcopy(buffer,msg,buffersize);
	msg[buffersize]=0;
	return msg;
}

char * default_encoder(char * msg, int * buffersize) {
	char * buffer;
	buffer=(char *) malloc(sizeof(char)*(strlen(msg)+1));
	*buffersize=strlen(msg);
	bcopy(msg,buffer,*buffersize);
	buffer[*buffersize]=0;
	return buffer;
}


typedef struct Context {
	int terminalfd;
	int output;
	int input;
	pid_t pid;
	BOOL started;
	struct termios inputopt;
	terminal_decoder decoder;
	terminal_encoder encoder;
} Context;

Context context;
context.decoder=&default_decoder;
context.encoder=&default_encoder;

/*
Action on SIGCHLD to manage Child termination
*/
static void received_sigchld_on_terminal(int sig) {
	int status,pid;
	//wait for one child process ends
	pid=wait(&status);
	if (pid==context.pid) {
		context.started=FALSE;
	}
}	

int winout;

/*
Action on SIGWINCH to manage TTY resizing
*/
static void resizeTTY(int sig) {
    struct winsize termsize;
	ioctl(context.output,TIOCGWINSZ,&termsize);
	ioctl(context.fd,TIOCSWINSZ,&termsize);
}


static void set_input_opt() {
	struct termios ttystate;

	// Backup intial TTY mode of input fd
	tcgetattr(context.input, &(context.inputopt));

	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(context.inputinput, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(context.input, TCSANOW, &ttystate);
}

static void restore_input_opt() {
	tcsetattr(context.input, TCSANOW, &(context.inputopt));
}

//SIGCHLD signal is sent on parent process who have a child process ending
//Parent process has to read the status of the child to liberate it.
//This function tell the parent to listen to the SIGCHLD signal.
static void listen_sigchld() {
	struct sigaction eventSigChld;
	//Create the sigaction structure to handle SIGCHLD signal
	sigemptyset(&eventSigChld.sa_mask);
	eventSigChld.sa_flags=0;
	eventSigChld.sa_handler= received_sigchld_on_terminal;
	//Start listening to SIGCHLD signal
	sigaction(SIGCHLD,&eventSigChld, NULL);
}



static void init_context(int input, int output) {
	context.started=FALSE;
	context.pid=-1;
	context.terminalfd=-1;
	context.input=input;
	context.output=output;
}

void resize_terminal(unsigned short row, unsigned short col) {
	struct winsize termsize;
	termsize.ws_row=row;
	termsize.ws_col=row;
	ioctl(context.terminalfd,TIOCSWINSZ,&termsize);
}

static int maxfd(int fd1, int fd2) {
	int result=0;
	if (fd1>fd2) {
		result=fd1+1;
	}else{
		result=fd2+1;
	}
	return result;
}
	
/*
Create a pty and fork the process to execute the bash --login, 
send all shell output on output FD and read input FD to send commands to the shell
*/
int start_terminal(int input, int output) {
	fd_set readset;
	int result;
	sigset_t mask;
	
	char buffer[100];
	int count;
	int status,pid;
	
	char * message;

	//Init the global context of the terminal
	init_context(input, output);
	
	
	//Define the sigmask  to catch SIGCHLD with pselect
	sigemptyset (&mask);
	sigaddset (&mask, SIGCHLD);

	//Create the fork PTY to manage our shell
	context.pid = forkpty(&(context.terminalfd), NULL, NULL, NULL);
	if (context.pid==-1) {
		return -1;
	} else if (context.pid == 0) {
		//here we are the child process
		char *argv[]={ "/bin/bash","--login", 0};
		execv(argv[0], argv);
		//child process ends here
	} else {
		/*
		//Create the sigaction structure to handle SIGWINCH signal
	    struct sigaction eventWindowResize;
		sigemptyset(&eventWindowResize.sa_mask);
		eventWindowResize.sa_flags=0;
		eventWindowResize.sa_handler= resizeTTY;

		//Change TTY size to the actual size of the terminal
		struct winsize termsize;
		ioctl(output,TIOCGWINSZ,&termsize);
		ioctl(child->fd,TIOCSWINSZ,&termsize);*/

		
		//Start handling signal
		//listen_sigchld();

		// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
		set_input_opt();

	
		//Initialize the flag to signal child is alive
		context.started=TRUE;
	
		do {
			//initialize file descriptor mask for select
			FD_ZERO(&readset);
			FD_SET(input, &readset);
			FD_SET(child->fd, &readset);
		
		
			//Block until something to read on child stdout, parent stdin or SIGCHLD
			result = pselect(maxfd(context.terminalfd, context.input), &readset, NULL, NULL, NULL, &mask);
			
			if (result > 0) {
				if (FD_ISSET(child->fd, &readset)) {
					//Child has write on its stdout
					count = read(child->fd, buffer, sizeof(buffer)-1);
					if (count >= 0) {
						buffer[count]=0;
						message=context.encoder(buffer, &count);
						if (message!=NULL) {
							write(context.output, message, count);
							free(message);
						}
					}
				}
				if (FD_ISSET(input, &readset)) {
					//User has write some stuff
					count = read(input, buffer, sizeof(buffer)-1);
					if (count >= 0) {
						message=context.decoder(buffer,count);
						if (message!=NULL) {
							write(context.terminalfd, message, strlen(message));
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
					if (pid==context.pid) {
						context.started=FALSE;
					}
				} else {
					//an error occurs, we quit 
					context.started=FALSE;
				}
			}
		} while (context.started);
		//reset input to its original mode and quit
		restore_input_opt();
	}
    return 1;
}

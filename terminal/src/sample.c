#include "termtty.h"

/*
Action on SIGWINCH to manage TTY resizing
*/
void resizeTTY(int sig) {
	struct winsize termsize;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&termsize);
	ioctl(fd,TIOCSWINSZ,&termsize);	
}	


int main() {

	//Create the sigaction structure to handle SIGWINCH signal
	struct sigaction eventWindowResize;
	sigemptyset(&eventWindowResize.sa_mask);
	eventWindowResize.sa_flags=0;
	eventWindowResize.sa_handler= resizeTTY;

	//Change TTY size to the actual size of the terminal
	struct winsize termsize;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&termsize);
	ioctl(fd,TIOCSWINSZ,&termsize);

	start_terminal(STDIN_FILENO, STDOUT_FILENO);
    return 1;
}
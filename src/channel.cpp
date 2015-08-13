#include "channel.h"
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

Channel::Channel(int input, int output) {
	this->input=input;
	this->output=output;
	::tcgetattr(input, &(this->normal));
}


Channel::~Channel() {
	::tcsetattr(this->input, TCSANOW, &(this->normal));
}

void Channel::modeNormal() {
	::tcsetattr(this->input, TCSANOW, &(this->normal));
}

void Channel::modePassword() {
	struct termios ttystate;
	::tcsetattr(this->input, TCSANOW, &(this->normal));
	::tcgetattr(this->input, &ttystate);
	ttystate.c_lflag &= ~ECHO;
	::tcsetattr(this->input, TCSANOW, &ttystate);
}

void Channel::modeTerminal() {
	struct termios ttystate;
	::tcsetattr(this->input, TCSANOW, &(this->normal));
	::tcgetattr(this->input, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	::tcsetattr(this->input, TCSANOW, &ttystate);

}


screensize Channel::termsize() {
	struct winsize termsize;
	ioctl(this->output,TIOCGWINSZ,&termsize);
	
	screensize size;
	size.h=termsize.ws_row;
	size.w=termsize.ws_col;
	return size;
}

int Channel::read(char * buffer, int size) {
	return ::read(this->input, buffer, size);
}

int Channel::write(char * buffer, int size) {
	return ::write(this->output, buffer, size);
}

int Channel::iread(char * buffer, int size) {
	int nb=0;
	fd_set readset;
	int result;
	sigset_t emptyset;
	FD_ZERO(&readset);	
	FD_SET(this->input, &readset);
	sigemptyset(&emptyset);
	result = pselect(this->input+1, &readset, NULL, NULL, NULL, &emptyset);
	if (result > 0) {
		if (FD_ISSET(this->input, &readset)) {
			nb=::read(this->input, buffer, size);
		}
	}
	return nb;
}

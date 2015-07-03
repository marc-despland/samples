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
#include <iostream>

#define BUFFERSIZE	1024
#define TERMINALBUFFERSIZE	256

const char* TermTTYForkException::what() {
	return "Can't fork process to create tty";
}


TermTTY::TermTTY(int input, int output):ForkPty(),Encoder() {
	this->setEncodedFd(input, output);
	struct termios ttystate;
	this->mask=new sigset_t();
	
	//Define the sigmask  to catch SIGCHLD with pselect
	sigemptyset (this->mask);
	sigaddset (this->mask, SIGCHLD);


	// Backup intial TTY mode of input fd
	tcgetattr(input, &(this->inputopt));

	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(input, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(input, TCSANOW, &ttystate);
	quit=NULL;
	try {
		quit=new Command(2,"");
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}


TermTTY::~TermTTY(){
	tcsetattr(this->encodedin, TCSANOW, &(this->inputopt));
	if (quit!=NULL) delete this->quit;
}

bool TermTTY::terminal(){
	try {
		this->execute();
		this->quit->send(this->encodedout);
	} catch(ForkPtyException &e) {
		return false;
	}
	return true;
}



void TermTTY::child() {
	cout << "start TermTTY child " << endl;
	char *argv[]={ "/bin/bash","--login", 0};
	execv(argv[0], argv);
	cout << "stop TermTTY child " << endl;
}



void TermTTY::parent() {
	cout << "start TermTTY parent " << endl;
	this->setClearFd(this->ptyfd,this->ptyfd);
	this->encode();	
	cout << "stop TermTTY parent " << endl;
}



void TermTTY::executecmd(Command * cmd) {
	switch (cmd->command()) {
		case 1: //resizeTTY
			struct winsize termsize;//ws_row ws_col
			cmd->parameters("%06u %06u",&(termsize.ws_row),&(termsize.ws_col));
			ioctl(this->ptyfd,TIOCSWINSZ,&termsize);
		break;
	}
}


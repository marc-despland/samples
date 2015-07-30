#include "client.h"
#include "command.h"
#include <sys/ioctl.h>
#include <iostream>

vector<Client *> Client::list;

/*
Action on SIGWINCH to manage TTY resizing
*/
static void received_SIGWINCH(int sig) {
	for(unsigned int i=0; i<Client::list.size();i++) {
		Client::list[i]->resizetty();
	}
}

static void received_SIGINT(int sig) {
	for(unsigned int i=0; i<Client::list.size();i++) {
		Client::list[i]->quit();
	}
}

void Client::setClearFd(int clearin, int clearout) {
	struct termios ttystate;

	// Backup intial TTY mode of input fd
	tcgetattr(clearin, &(this->inputopt));

	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(clearin, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(clearin, TCSANOW, &ttystate);
	Encoder::setClearFd(clearin, clearout);
}

	
Client::Client():Runnable(),Encoder(this,"Client") {
	this->mask=new sigset_t();
	//Define the sigmask  to catch SIGWINCH with pselect
	sigemptyset (this->mask);
	sigaddset (this->mask,SIGWINCH);
	sigaddset (this->mask,SIGINT);
	sigprocmask(SIG_BLOCK, this->mask, NULL);
	
	this->index=Client::list.size();
	cout << "Creating the client " << this->index << endl;
	Client::list.push_back(this);
	
	
	this->eventWindowResize=new struct sigaction();
	sigemptyset(&(this->eventWindowResize->sa_mask));
	this->eventWindowResize->sa_flags=0;
	this->eventWindowResize->sa_handler= received_SIGWINCH;
	
	sigaction(SIGWINCH,this->eventWindowResize, NULL);


	this->eventInterrupted=new struct sigaction();
	sigemptyset(&(this->eventInterrupted->sa_mask));
	this->eventInterrupted->sa_flags=0;
	this->eventInterrupted->sa_handler= received_SIGINT;
	
	sigaction(SIGINT,this->eventInterrupted, NULL);

}

Client::~Client() {
	cout << "Deleting the client " << this->index << endl;
	if (this->clearin>=0) tcsetattr(this->clearin, TCSANOW, &(this->inputopt));
	Client::list.erase(Client::list.begin()+this->index);
	delete this->eventWindowResize;
	delete this->eventInterrupted;
}

void Client::executecmd(Command * cmd) {
	switch (cmd->command()) {
		case 2: //quit
			this->stop();
		break;
	}
}

void Client::resizetty() {
	cout << "Client received resize screen" << endl;
	struct winsize termsize;
	ioctl(this->clearout,TIOCGWINSZ,&termsize);
	try {
		Command cmd(1,"%06u %06u",termsize.ws_row,termsize.ws_col);
		cmd.send(this->encodedout);
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}

void Client::quit() {
	cout << "Client received quit" << endl;
	try {
		Command cmd(2,"");
		cmd.send(this->encodedout);
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}


bool Client::run() {
	this->resizetty();
	try {
		this->encode();
	}catch(EncoderInvalidFdException &e) {
		return false;
	}
	return true;
}


#include "client.h"
#include "command.h"
#include <sys/ioctl.h>

vector<Client *> Client::list;

/*
Action on SIGWINCH to manage TTY resizing
*/
static void received_SIGWINCH(int sig) {
	for(unsigned int i=0; i<Client::list.size();i++) {
		Client::list[i]->resizetty();
	}
}



Client::Client(int clin, int clout, int termin, int termout):Encoder() {
	this->setClearFd(clin, clout);
	this->setEncodedFd(termin, termout);
	
	struct termios ttystate;

	// Backup intial TTY mode of input fd
	tcgetattr(clin, &(this->inputopt));

	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(clin, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(clin, TCSANOW, &ttystate);
	
	this->mask=new sigset_t();
	//Define the sigmask  to catch SIGWINCH with pselect
	sigemptyset (this->mask);
	sigaddset (this->mask,SIGWINCH);
	
	this->index=Client::list.size();
	Client::list.push_back(this);
	
	
	this->eventWindowResize=new struct sigaction();
	sigemptyset(&(this->eventWindowResize->sa_mask));
	this->eventWindowResize->sa_flags=0;
	this->eventWindowResize->sa_handler= received_SIGWINCH;
	
	sigaction(SIGWINCH,this->eventWindowResize, NULL);

}

Client::~Client() {
	tcsetattr(this->encodedin, TCSANOW, &(this->inputopt));
	Client::list.erase (Client::list.begin()+this->index);
	delete this->eventWindowResize;
}

void Client::executecmd(Command * cmd) {
	switch (cmd->command()) {
		case 2: //quit
			this->stop();
		break;
	}
}

void Client::resizetty() {
	struct winsize termsize;
	ioctl(this->clearout,TIOCGWINSZ,&termsize);
	try {
		Command cmd(1,"%06u %06u",termsize.ws_row,termsize.ws_col);
		cmd.send(this->encodedin);
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


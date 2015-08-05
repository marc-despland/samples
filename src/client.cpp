#include "client.h"
#include "command.h"
#include <sys/ioctl.h>
#include <iostream>
#include "log.h"

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
	Log::logger->log("CLIENT",DEBUG) << "SIGINT Detected" << endl;
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
	Log::logger->log("CLIENT",DEBUG) << "Creating the client " << this->index << endl;
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
	Log::logger->log("CLIENT",DEBUG) << "Deleting the client " << this->index << endl;
	if (this->clearin>=0) tcsetattr(this->clearin, TCSANOW, &(this->inputopt));
	Client::list.erase(Client::list.begin()+this->index);
	delete this->eventWindowResize;
	delete this->eventInterrupted;
}

void Client::executecmd(Command * cmd) {
	Log::logger->log("CLIENT",DEBUG) << "Client Received a command : " << cmd->command() << endl;
	switch (cmd->command()) {
		case Command::QUIT: //quit
			this->stop();
		break;
		case Command::REQUESTTTYSIZE:
			this->resizetty();
		break;
		case Command::REQUESTAUTHENT: 
			try {
				this->requestauthent();
			} catch(InterruptAuthentException &e) {
				Log::logger->log("CLIENT",DEBUG) << "Authent process interrupted" << endl;
			}
		break;
		case Command::FAILEDAUTHENT:
			try {
				this->requestauthent();
			} catch(InterruptAuthentException &e) {
				Log::logger->log("CLIENT",DEBUG) << "Authent process interrupted" << endl;
			}
		break;
	}
}

void Client::resizetty() {
	Log::logger->log("CLIENT",DEBUG) << "Client received resize screen" << endl;
	struct winsize termsize;
	ioctl(this->clearout,TIOCGWINSZ,&termsize);
	try {
		Command cmd(Command::RESIZETTY,"%06u %06u",termsize.ws_row,termsize.ws_col);
		cmd.send(this->encodedout);
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}

void Client::quit() {
	Log::logger->log("CLIENT",DEBUG) << "Client received quit" << endl;
	try {
		Command cmd(Command::QUIT,"");
		cmd.send(this->encodedout);
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}


bool Client::run() {
	//this->resizetty();
	try {
		this->encode();
	}catch(EncoderInvalidFdException &e) {
		return false;
	}
	return true;
}

void Client::requestauthent() throw(InterruptAuthentException){
	Log::logger->log("CLIENT",DEBUG) << "Server request authentication" << endl;
	struct termios termopt;
	struct termios pwdopt;
	string login;
	string password;
	char buffer[100];
	// Backup blind TTY mode of input fd
	tcgetattr(this->clearin, &(termopt));
	
	//restore default mode
	tcsetattr(this->clearin, TCSANOW, &(this->inputopt));
	write(this->clearout, "Login:", 6);
	int nb=this->clearread(buffer, 100);
	if (nb==0) throw InterruptAuthentException();
	buffer[nb]=0;
	login=buffer;
	//cin >> login;
	write(this->clearout, "Password:", 9);
	// Update input mode to remove ECHO and ICANON to allow transmission of character without buffering and echo
	tcgetattr(this->clearin, &pwdopt);
	pwdopt.c_lflag &= ~ECHO;
	tcsetattr(this->clearin, TCSANOW, &pwdopt);
	nb=this->clearread(buffer, 100);
	if (nb==0) throw InterruptAuthentException();
	buffer[nb]=0;
	password=buffer;
	write(this->clearout, "\n", 1);
	//restore terminal option
	tcsetattr(clearin, TCSANOW, &(termopt));
	Command auth(Command::AUTHENTICATE,"%s %s",login.c_str(), password.c_str());
	auth.send(this->encodedout);
}

int Client::clearread(char * buffer, int size) {
	int nb=0;
	fd_set readset;
	int result;
	sigset_t emptyset;
	FD_ZERO(&readset);	
	FD_SET(this->clearin, &readset);
	sigemptyset(&emptyset);
	result = pselect(this->clearin+1, &readset, NULL, NULL, NULL, &emptyset);
	if (result > 0) {
		if (FD_ISSET(this->clearin, &readset)) {
			nb=read(this->clearin, buffer, size);
		}
	}
	return nb;
}


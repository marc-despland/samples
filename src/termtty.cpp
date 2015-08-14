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
#include "log.h"

#define BUFFERSIZE	1024
#define TERMINALBUFFERSIZE	256

const char* TermTTYForkException::what() {
	return "Can't fork process to create tty";
}
void TermTTY::setUser(uid_t uid, gid_t gid) {
	this->uid=uid;
	this->gid=gid;
}

TermTTY::TermTTY(int input, int output):ForkPty(),Encoder("Server") {
	this->uid=-1;
	this->gid=-1;
	this->setEncodedFd(input, output);
	/*struct termios ttystate;
	this->mask=new sigset_t();
	//Define the sigmask  to catch SIGCHLD with pselect
	sigemptyset (this->mask);
	sigaddset (this->mask, SIGCHLD);*/

	this->encoded->modeTerminal();
	quit=NULL;
	try {
		quit=new Command(2,"");
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}


TermTTY::~TermTTY(){
	this->encoded->modeNormal();
	//tcsetattr(this->encodedin, TCSANOW, &(this->inputopt));
	if (quit!=NULL) delete this->quit;
}

bool TermTTY::terminal(){
	try {
		this->execute();
		this->quit->send(this->encoded);
	} catch(ForkPtyException &e) {
		return false;
	}
	return true;
}



void TermTTY::child() {
	bool ok=true;
	Log::logger->log("TTY", DEBUG) << "Child TTY created" << endl;
	if (this->uid>=0) {
		Log::logger->log("TTY", DEBUG) << "We need to switch to user ("<<this->uid<<","<<this->gid<<")" << endl;
		ok=(setgid(this->gid)==0 && setuid(this->uid)==0);
	
	}
	if (ok) {
		Log::logger->log("TTY", DEBUG) << "Create the pty to execute shell" << endl;
		char * const argv[]={ "/bin/bash","--login", 0};
		execv(argv[0], argv);
	}
}



void TermTTY::parent() {
	Log::logger->log("TTY", DEBUG) << "Start to manage communication" << endl;
	this->setClearFd(this->ptyfd,this->ptyfd);
	Command cmd(Command::REQUESTTTYSIZE, "");
	cmd.send(this->encoded);
	this->encode();	
}



void TermTTY::executecmd(Command * cmd) {
	Log::logger->log("TTY", NOTICE)  << "We receive a command " << cmd->command()<< endl;
	switch (cmd->command()) {
		case Command::RESIZETTY: //resizeTTY
			unsigned int h, w;
			cmd->parameters("%06u %06u",&(h),&(w));
			this->clear->window(w,h);
		break;
		case Command::QUIT: //quit
			this->stop();
		case Command::REQUESTTTYSIZE: //request size
			//nothing to do on server side
		break;
	}
}


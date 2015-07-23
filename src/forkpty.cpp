#include "forkpty.h"
#include <sys/wait.h>
#include <pty.h>
#include <iostream>

map<pid_t, ForkPty *>	ForkPty::processlist;

static void received_sigchld(int sig) {
	int status,pid;
	//wait for one child process ends
	pid=wait(&status);
	if (ForkPty::processlist.count(pid)==1) {
		ForkPty::processlist[pid]->stop();
	}
}

ForkPty::ForkPty() {
	struct sigaction eventSigChld;
	//Create the sigaction structure to handle SIGCHLD signal
	sigemptyset(&eventSigChld.sa_mask);
	eventSigChld.sa_flags=0;
	eventSigChld.sa_handler= received_sigchld;
	//Start listening to SIGCHLD signal
	sigaction(SIGCHLD,&eventSigChld, &(this->oldact));
}

ForkPty::~ForkPty() {
	sigaction(SIGCHLD,&(this->oldact), NULL);
}

 
void ForkPty::execute() throw (ForkPtyException) {
	cout << "ForkPty execute " << endl;
	if((this->childpid = forkpty(&(this->ptyfd),NULL,NULL,NULL)) == -1) throw ForkPtyException();
	if(this->childpid == 0) {
		this->child();
	} else {
		this->start();
		processlist[this->childpid]=this;
		this->parent();
		processlist.erase(this->childpid);
	}
}

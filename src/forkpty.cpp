#include "forkpty.h"
#include <sys/wait.h>
#include <pty.h>
#include <iostream>
#include "log.h"

map<pid_t, ForkPty *>	ForkPty::processlist;

void ForkPty::sigchld(int sig) {
	int status,pid;
	//wait for one child process ends
	pid=wait(&status);
	Log::logger->log("TTY", DEBUG) << "Received sigchld on ForkPty for process "<< pid << endl;
	if (ForkPty::processlist.count(pid)==1) {
		ForkPty::processlist.erase(pid);
	}
}

ForkPty::ForkPty() {
}

ForkPty::~ForkPty() {
	//sigaction(SIGCHLD,&(this->oldact), NULL);
}

 
void ForkPty::execute() throw (ForkPtyException) {
	Log::logger->log("TTY", DEBUG) << "ForkPty execute " << endl;
	if((this->childpid = forkpty(&(this->ptyfd),NULL,NULL,NULL)) == -1) throw ForkPtyException();
	if(this->childpid == 0) {
		Log::logger->log("TTY", DEBUG) << "ForkPty fork done, we are the child " << endl;
		this->child();
	} else {
		Log::logger->log("TTY", DEBUG) << "ForkPty fork done, we are the parent " << endl;
		//Only the parent need to listen to SIGCHLD signal
		::signal(SIGCHLD, ForkPty::sigchld);

		processlist[this->childpid]=this;
		this->parent();
		processlist.erase(this->childpid);
	}
}

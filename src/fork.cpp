#include "fork.h"
#include <sys/wait.h>
#include <iostream>
#include <stdlib.h>

map<pid_t, Fork *>	Fork::processlist;

static void received_sigchld(int sig) {
	int status,pid;
	//wait for one child process ends
	pid=wait(&status);
	cout << "received sigchld from " <<pid <<endl;
	if (Fork::processlist.count(pid)==1) {
		Fork::processlist[pid]->stop();
		Fork::processlist.erase(pid);
	}
}

Fork::Fork():Runnable() {
	struct sigaction eventSigChld;
	//Create the sigaction structure to handle SIGCHLD signal
	sigemptyset(&eventSigChld.sa_mask);
	eventSigChld.sa_flags=0;
	eventSigChld.sa_handler= received_sigchld;
	//Start listening to SIGCHLD signal
	sigaction(SIGCHLD,&eventSigChld, &(this->oldact));
}

Fork::~Fork() {
	sigaction(SIGCHLD,&(this->oldact), NULL);
}

 
void Fork::execute() throw (ForkException) {
	if((this->childpid = fork()) == -1) throw ForkException();
	if(this->childpid == 0) {
		this->child();
		exit(0);
	} else {
		this->start();
		processlist[this->childpid]=this;
		this->parent();
	}
}

#include "fork.h"
#include <sys/wait.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include "log.h"

map<pid_t, Fork *>	Fork::processlist;

Fork * Fork::thechild=NULL;

void Fork::sigint(int sig) {
	Log::logger->log("FORK", DEBUG)<< "Kill detected by child " << endl;
	pid_t pid=getpid();
	Fork::thechild->status->stop();
	if (Fork::processlist.count(pid)==1) {
		Log::logger->log("FORK", DEBUG) << "Stoping status child " << getpid() << endl;
		Fork::processlist[pid]->status->stop();
	}
}



void Fork::sigchld(int sig) {
	int status;
	pid_t pid;
	//wait for one child process ends
	pid=wait(&status);
	Log::logger->log("FORK", DEBUG) << "Received sigchld from process " <<pid <<endl;
	if (Fork::processlist.count(pid)==1) {
		Fork::processlist.erase(pid);
	}
}


Fork::Fork(IRunnable * status) {
	this->status=status;
	this->childpid=-1;
}

Fork::~Fork() {
	this->status->stop();
	sigaction(SIGCHLD,&(this->oldact), NULL);
}

void Fork::killChild() {
	if (this->childpid>0) {
		this->status->stop();
		Log::logger->log("FORK", DEBUG) << "Send SIGINT to child : " << this->childpid << endl;
		kill(this->childpid, SIGINT);
	}
}

 
void Fork::execute() throw (ForkException) {
	Log::logger->log("FORK", DEBUG) << "Fork execute " << endl;
	if((this->childpid = fork()) == -1) throw ForkException();
	if(this->childpid == 0) {
		Log::logger->log("FORK", DEBUG) << "Fork done, we are the child " << endl;
		thechild=this;
		struct sigaction eventSigInt;
		//Create the sigaction structure to handle SIGINT signal
		sigemptyset(&eventSigInt.sa_mask);
		eventSigInt.sa_flags=0;
		eventSigInt.sa_handler= Fork::sigint;
		//Start listening to SIGINT signal
		sigaction(SIGINT,&eventSigInt, NULL);

		this->child();
		exit(0);
	} else {
		Log::logger->log("FORK", DEBUG) << "Fork done, we are the parent" << endl;

		struct sigaction eventSigChld;
		//Create the sigaction structure to handle SIGCHLD signal
		sigemptyset(&eventSigChld.sa_mask);
		eventSigChld.sa_flags=0;
		eventSigChld.sa_handler= Fork::sigchld;
		//Start listening to SIGCHLD signal
		sigaction(SIGCHLD,&eventSigChld, &(this->oldact));

		this->status->start();
		Fork::processlist[this->childpid]=this;
		this->parent();
	}
}

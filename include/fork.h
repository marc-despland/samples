#ifndef _FORK_H
#define _FORK_H
using namespace std;
#include <exception>
#include <map>
#include <unistd.h>
#include <signal.h>
#include "irunnable.h"

class ForkException : public exception {
	const char* what();
};

class Fork {
	public :
		Fork(IRunnable * status);
		virtual ~Fork();
		void execute() throw (ForkException);
		void killChild();
		static map<pid_t, Fork *>	processlist;
		
		static void sigint(int signal);
		static void sigchld(int sig);
	protected:
		struct sigaction oldact;
		pid_t   childpid;
		virtual void child()=0;
		virtual void parent()=0;
		IRunnable * status;
		static Fork * thechild;

};
#endif

#ifndef _FORKPTY_H
#define _FORKPTY_H
using namespace std;
#include <exception>
#include <map>
#include <unistd.h>
#include <signal.h>
#include "irunnable.h"

class ForkPtyException : public exception {
	const char* what();
};

class ForkPty {
	public :
		ForkPty();
		virtual ~ForkPty();
		void execute() throw (ForkPtyException);
		static map<pid_t, ForkPty *>	processlist;
		static void sigchld(int sig);
	protected:
		struct sigaction oldact;
		pid_t   childpid;
		int 	ptyfd;
		virtual void child()=0;
		virtual void parent()=0;
};
#endif

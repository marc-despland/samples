#ifndef _FORK_H
#define _FORK_H
using namespace std;
#include <exception>
#include <map>
#include <unistd.h>
#include <signal.h>
#include "runnable.h"

class ForkException : public exception {
	const char* what();
};

class Fork :public virtual Runnable{
	public :
		Fork();
		virtual ~Fork();
		void execute() throw (ForkException);
		static map<pid_t, Fork *>	processlist;
	protected:
		struct sigaction oldact;
		pid_t   childpid;
		virtual void child()=0;
		virtual void parent()=0;

};
#endif

#ifndef _TERMTTY_H
#define _TERMTTY_H
using namespace std;
#include <exception>
#include "buffer.h"
#include "packet.h"
#include <unistd.h>
#include <termios.h>
#include "forkpty.h"
#include "encoder.h"
#include "command.h"
#include "irunnable.h"

class TermTTYForkException : public exception {
	const char* what();
};


class TermTTY :  public ForkPty, public Encoder {
	public:
		TermTTY(IRunnable * status, int input, int output);
		virtual ~TermTTY();
		bool terminal();
	protected:
		void executecmd(Command * cmd);
		struct termios inputopt;
		Command * quit;
		void child();
		void parent();
		IRunnable * status;
};


#endif

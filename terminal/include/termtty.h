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

class TermTTYForkException : public exception {
	const char* what();
};


class TermTTY :  public ForkPty, public Encoder {
	public:
		TermTTY(int input, int output);
		~TermTTY();
		bool terminal();
	protected:
		void executecmd(Command * cmd);
		struct termios inputopt;
		Command * quit;
		void child();
		void parent();
};


#endif

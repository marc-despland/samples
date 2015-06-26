#ifndef _TERMTTY_H
#define _TERMTTY_H
using namespace std;
#include <exception>
#include "buffer.h"
#include <unistd.h>
#include <termios.h>


class TermTTYForkException : public exception {
	const char* what();
};


class TermTTY {
	public:
		TermTTY(int input, int output);
		~TermTTY();
		bool execute()  throw (TermTTYForkException);
	protected:
		Buffer * buffer;
		int pid;
		int terminalfd;
		int input;
		int output;
		bool started;
		struct termios inputopt;
};


#endif

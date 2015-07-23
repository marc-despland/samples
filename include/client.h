#ifndef _CLIENT_H
#define _CLIENT_H
#include <exception>
using namespace std;
#include "encoder.h"
#include <unistd.h>
#include <termios.h>
#include <vector>

class Client:public Encoder {
	public:
		Client(int clin, int clout, int termin, int termout);
		~Client();
		void executecmd(Command * cmd);
		void resizetty();
		static vector<Client *> list;
	 	bool run();
	protected:
		struct termios inputopt;
		struct sigaction * eventWindowResize;
		int index;
		

};
#endif

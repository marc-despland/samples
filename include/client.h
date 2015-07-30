#ifndef _CLIENT_H
#define _CLIENT_H
#include <exception>
using namespace std;
#include "encoder.h"
#include <unistd.h>
#include <termios.h>
#include <vector>
#include "runnable.h"

class Client:public Runnable, public Encoder {
	public:
		//Client(int clin, int clout, int termin, int termout);
		Client();
		virtual ~Client();
		void setClearFd(int clearin, int clearout);
		void executecmd(Command * cmd);
		void resizetty();
		void quit();
		static vector<Client *> list;
	 	bool run();
	protected:
		struct termios inputopt;
		struct sigaction * eventWindowResize;
		struct sigaction * eventInterrupted;
		int index;
		

};
#endif

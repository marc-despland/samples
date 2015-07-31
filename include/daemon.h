#ifndef _DAEMON_H
#define _DAEMON_H
#include "options.h"
#include "parameters.h"

class Daemon {

	public:
		Daemon(string program, string version, string description);
		virtual ~Daemon();
		void start(int argc, char **argv) throw (OptionsStopException, UnknownOptionException, CantCreateFileException);
		
	protected:
		virtual void daemon()=0;
		
		Options *options;
		Parameters * parameters;
		std::filebuf * logfile;


};

#endif

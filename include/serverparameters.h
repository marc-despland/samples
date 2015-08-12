#ifndef _SERVERPARAMETERS_H
#define _SERVERPARAMETERS_H
#include <string>
using namespace std;
#include "connectiontarget.h"

class ServerParameters {
	public:
		ServerParameters(ConnectionTarget * target);
		virtual ~ServerParameters();
		unsigned int pool();
		void pool(unsigned int);
		ConnectionTarget * listenOn();
	protected:
		
		unsigned int poolsize;
		unsigned int cnxtype;
		ConnectionTarget * target;
};


#endif
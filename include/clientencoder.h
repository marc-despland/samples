#ifndef _CLIENTENCODER_H
#define _CLIENTENCODER_H
#include "client.h"
#include "encoder.h"

#include <exception>
using namespace std;

class InterruptAuthentException: public exception {};

template <typename Cnx>   
class ClientEncoder: public Client<Cnx>, public Encoder {
	public:
		virtual ~ClientEncoder();
		static ClientEncoder * client();
		void executecmd(Command * cmd);
		void ioChannel(Channel * clear);
	
	protected:
		void execute();
		ClientEncoder();
		static ClientEncoder * theclient;
		static void signalHandler(int sig);
		void authenticate() throw (InterruptAuthentException);
		void resizetty();
		
	
};

#endif
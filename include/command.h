#ifndef _COMMAND_H
#define _COMMAND_H

#include <string>
using namespace std;

#include <exception>
#include <stdarg.h>
#include "packet.h"



class InvalidCommandException : public exception {
	const char* what();
};

class InvalidCmdCodeException : public exception {
	const char* what();
};


class Command : public Packet {
	protected:
		unsigned int cmd;
	public:
		/**
		 * Create a new empty Command
		 */
		Command();
		Command(unsigned int code, const char * format, ...) throw(InvalidCmdCodeException);
		Command(Packet * packet) throw(InvalidCommandException, PacketNotReadyException, InvalidCmdCodeException);
		virtual ~Command();
		
		unsigned int command();
		int parameters(const char * format, ...);
		
		static bool isCommand(Packet * packet);
		static const unsigned short CODECOMMAND=0;
		static const unsigned short NOCMD=0;
};
#endif 

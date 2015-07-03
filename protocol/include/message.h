#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
using namespace std;

#include <exception>
#include "packet.h"



class InvalidMessageException : public exception {
	const char* what();
};


/**
 * Packet class manage based data exchange between client and server. 
 * Packet is composed of a code that describe the type of packet, a 
 * length encoded on 4 digit that decribe the size of the data, and a block of data.
 */
class Message : public Packet {
	public:
		/**
		 * Create a new empty Message
		 */
		Message();
		Message(string str);
		Message(const char str[]);
		Message(Packet packet) throw(InvalidMessageException);
		char * message()throw (PacketNotReadyException);
		long sendmsg(int fd) throw (PacketNotReadyException);
		static bool isMessage(Packet * packet);
		static const unsigned short CODEMESSAGE=1;
};
#endif 

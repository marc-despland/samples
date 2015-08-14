#ifndef _ENCODER_H
#define _ENCODER_H
#include <signal.h>
#include <exception>
#include "command.h"
#include "message.h"
#include "runnable.h"
#include "channel.h"

using namespace std;

class EncoderInvalidFdException : public exception {
	const char* what();
};
class EncoderStreamException : public exception {
	const char* what();
};

class Encoder : virtual public Runnable{
	public:
		/**
		 * encoder work :
		 *	clearin 	->	encodedout
		 *	encodedin	->	clearout
		 */
		Encoder(string name);
		virtual ~Encoder();
		void setMask(sigset_t * mask);
		void setClearFd(int clearin, int clearout);
		void setEncodedFd(int encodedin, int encodedout);
		void channelClear(Channel * clear);
		void channelEncoded(Channel * encoded);
		void encode() throw(EncoderInvalidFdException);
	protected:
		void readclear() throw(EncoderStreamException);
		void readencoded() throw(EncoderStreamException);
		virtual void executecmd(Command * cmd)=0;

	protected:
		string name;
		Buffer * 	bufferencoded;
		Packet *	packet;
		Channel * clear;
		Channel * encoded;
		sigset_t * mask;

};

#endif

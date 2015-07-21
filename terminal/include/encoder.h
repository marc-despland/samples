#ifndef _ENCODER_H
#define _ENCODER_H
#include <signal.h>
#include <exception>
#include "command.h"
#include "message.h"
#include "runnable.h"
using namespace std;

class EncoderInvalidFdException : public exception {
	const char* what();
};

class Encoder : public virtual Runnable {
	public:
		/**
		 * encoder work :
		 *	clearin 	->	encodedout
		 *	encodedin	->	clearout
		 */
		Encoder(string name);
		~Encoder();
		void setMask(sigset_t * mask);
		void setClearFd(int clearin, int clearout);
		void setEncodedFd(int encodedin, int encodedout);
		void encode() throw(EncoderInvalidFdException);
	protected:
		void readclear();
		void readencoded();
		virtual void executecmd(Command * cmd)=0;

	protected:
		string name;
		Buffer * 	bufferencoded;
		Packet *	packet;
		int clearin;
		int clearout;
		int encodedin;
		int encodedout;
		sigset_t * mask;

};

#endif

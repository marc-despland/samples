#include "encoder.h"

#include <signal.h>
#include <errno.h>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define BUFFERSIZE	256


Encoder::Encoder(string name):Runnable(){
	this->clearin=-1;
	this->clearout=-1;
	this->encodedin=-1;
	this->encodedout=-1;
	this->name=name;
}

void Encoder::setMask(sigset_t * mask) {
	this->mask=mask;
}	

void Encoder::setClearFd(int clearin, int clearout) {
	this->clearin=clearin;
	this->clearout=clearout;
}

void Encoder::setEncodedFd(int encodedin, int encodedout) {
	this->encodedin=encodedin;
	this->encodedout=encodedout;
}

Encoder::~Encoder(){
	delete this->bufferencoded;
	delete this->packet;
}

void Encoder::encode() throw (EncoderInvalidFdException){
	fd_set readset;
	int result;
	if ((this->clearin<0) || (this->clearout<0) || (this->encodedin<0) || (this->encodedout<0)) {
		throw EncoderInvalidFdException();
	}
	
	this->bufferencoded=new Buffer();
	this->packet=new Packet();

	this->start();
	do {
	
		FD_ZERO(&readset);	
		FD_SET(this->clearin, &readset);
		FD_SET(this->encodedin, &readset);		
		
		result = pselect(max(this->clearin, this->encodedin)+1, &readset, NULL, NULL, NULL, this->mask);
		if (result > 0) {
			if (FD_ISSET(this->encodedin, &readset)) {
				this->readencoded();
			}
			if (FD_ISSET(this->clearin, &readset)) {
				this->readclear();
			}
		} else {
			if (errno!=EINTR) {
				//an error occurs, we quit 
				this->stop();
			}
		}
	} while(this->running());	
}


void Encoder::readclear() {
	char buffer[BUFFERSIZE];
	int count;
	//Child has write on its stdout
	count = read(this->clearin, buffer, BUFFERSIZE-1);
	if (count >= 0) {
		buffer[count]=0;
		Message * out=new Message(buffer);
		try {
			out->send(this->encodedout);
			delete out;
		}catch(PacketNotReadyException &e) {
			//impossible
		}
	}
}


void Encoder::readencoded() {
	char buffer[BUFFERSIZE];
	int count;
	//User has write some stuff
	count = read(this->encodedin, buffer, BUFFERSIZE-1);
	if (count >= 0) {
		bool somethingToRead=true;
		this->bufferencoded->write(buffer, count);
		while (this->bufferencoded->size()>0 && somethingToRead) {
			try {
				this->packet->readData(this->bufferencoded);
			} catch(PacketInvalidHeaderException &e) {
				this->bufferencoded->clear();
				this->packet->clear();
			} catch(PacketBufferSizeException &e) {
				somethingToRead=false;
			}
			if (this->packet->isReady()) {
				switch(this->packet->getCode()) {
					case Message::CODEMESSAGE:
						try {
							Message * msg=new Message(this->packet);
							msg->sendmsg(this->clearout);
							this->packet->clear();
							delete msg;
						}catch(PacketNotReadyException &e) {
							//impossible
						}catch(InvalidMessageException &e) {
							//impossible
						}
					break;
					case Command::CODECOMMAND:
						try {
							Command * cmd=new Command(this->packet);
							this->executecmd(cmd);
							this->packet->clear();
							delete cmd;
						}catch(PacketNotReadyException &e) {
							//impossible	
						}catch(InvalidCommandException &e) {
							//impossible
						}catch(InvalidCmdCodeException &e) {
							packet->clear();
						}
					break;
				}
			}

		}
	}
}



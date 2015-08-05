#include "encoder.h"

#include <signal.h>
#include <errno.h>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "log.h"

#define BUFFERSIZE	256


Encoder::Encoder(IRunnable * status,string name){
	this->clearin=-1;
	this->clearout=-1;
	this->encodedin=-1;
	this->encodedout=-1;
	this->name=name;
	this->bufferencoded=NULL;
	this->packet=NULL;
	this->status=status;
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
	if (this->bufferencoded!=NULL) delete this->bufferencoded;
	if (this->packet!=NULL) delete this->packet;
}

void Encoder::encode() throw (EncoderInvalidFdException){
	fd_set readset;
	int result;
	sigset_t emptyset;
	bool fdisset=false;
/*	if ((this->clearin<0) || (this->clearout<0) || (this->encodedin<0) || (this->encodedout<0)) {
		throw EncoderInvalidFdException();
	}*/
	
	this->bufferencoded=new Buffer();
	this->packet=new Packet();

	this->status->start();
	do {
	
		FD_ZERO(&readset);	
		if (this->clearin>=0) FD_SET(this->clearin, &readset);
		FD_SET(this->encodedin, &readset);		
		sigemptyset(&emptyset);
		result = pselect(max(this->clearin, this->encodedin)+1, &readset, NULL, NULL, NULL, &emptyset);
		if (result > 0) {
			fdisset=false;
			try {
				if (FD_ISSET(this->encodedin, &readset)) {
					Log::logger->log("ENCODER", DEBUG) <<  "Read encodeded data" << endl;
					this->readencoded();
					fdisset=true;
				}
				if (FD_ISSET(this->clearin, &readset)) {
					Log::logger->log("ENCODER", DEBUG) <<  "Read clear data" << endl;
					this->readclear();
					fdisset=true;
				}
				if (!fdisset) {
					Log::logger->log("ENCODER", DEBUG) << "No FD is set, probably have to stop" << endl;
				}
			}catch(EncoderStreamException &e){
				Log::logger->log("ENCODER", ERROR) << "One of the stream failed : stop " << endl;
				this->status->stop();
			}
		} else {
			if (errno!=EINTR) {
				//an error occurs, we quit 
				Log::logger->log("ENCODER", ERROR) << "An error occurs, have to quit" << endl;
				this->status->stop();
			} else {
				Log::logger->log("ENCODER", DEBUG) << " Received EINTR in Encoder loop " << endl;
			}
		}
	} while(this->status->running());
	Log::logger->log("ENCODER", DEBUG) << "Encoder loop ended" << endl;	
}


void Encoder::readclear() throw (EncoderStreamException) {
	Log::logger->log("ENCODER", DEBUG) << "Enter readclear" << endl;
	char buffer[BUFFERSIZE];
	int count;
	//Child has write on its stdout
	count = read(this->clearin, buffer, BUFFERSIZE-1);
	if (count > 0) {
		buffer[count]=0;
		Log::logger->log("ENCODER", DEBUG) << "Read: " << buffer<< endl;
		Message * out=new Message(buffer);
		try {
			out->send(this->encodedout);
			delete out;
		}catch(PacketNotReadyException &e) {
			//impossible
		}
	} else {
		Log::logger->log("ENCODER", DEBUG) << "Strange, nothing to read on clear fd" << endl;
		throw EncoderStreamException();
	}
}


void Encoder::readencoded()  throw (EncoderStreamException)  {
	char buffer[BUFFERSIZE];
	int count;
	//User has write some stuff
	count = read(this->encodedin, buffer, BUFFERSIZE-1);
	if (count > 0) {
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
							if (this->clearout>=0) {
								Message * msg=new Message(this->packet);
								msg->sendmsg(this->clearout);
								this->packet->clear();
								delete msg;
							} else {
								this->packet->clear();
							}
							
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
	} else {
		Log::logger->log("ENCODER", DEBUG) << "Strange, nothing to read on encoded fd" << endl;
		throw EncoderStreamException();
	}
}



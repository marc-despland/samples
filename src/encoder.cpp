#include "encoder.h"

#include <signal.h>
#include <errno.h>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "log.h"


#define BUFFERSIZE	256


Encoder::Encoder(string name):Runnable(){
	this->clear=NULL;
	this->encoded=NULL;
	this->name=name;
	this->bufferencoded=NULL;
	this->packet=NULL;
}

void Encoder::setMask(sigset_t * mask) {
	this->mask=mask;
}

void Encoder::channelClear(Channel * clear) {
	this->clear=clear;
}
void Encoder::channelEncoded(Channel * encoded) {
	this->encoded=encoded;
}

void Encoder::setClearFd(int clearin, int clearout) {
	this->clear=new Channel(clearin, clearout);
}

void Encoder::setEncodedFd(int encodedin, int encodedout) {
	this->encoded=new Channel(encodedin, encodedout);
}

Encoder::~Encoder(){
	if (this->bufferencoded!=NULL) delete this->bufferencoded;
	if (this->packet!=NULL) delete this->packet;
}

void Encoder::encode() throw (EncoderInvalidFdException){

	this->bufferencoded=new Buffer();
	this->packet=new Packet();
	this->start();
	Watcher * watcher=new Watcher();
	if (this->clear!=NULL) watcher->watch(this->clear);
	if (this->encoded!=NULL) watcher->watch(this->encoded);
	try {
		do {
			try {
				vector<Channel *> ready=watcher->select();
				for (unsigned int i=0; i<ready.size();i++) {
					if (ready[i]==this->clear) {
						Log::logger->log("ENCODER", DEBUG) <<  "Read clear data" << endl;
						this->readclear();
					}
					if (ready[i]==this->encoded) {
						Log::logger->log("ENCODER", DEBUG) <<  "Read encodeded data" << endl;
						this->readencoded();
					}				
				}
			} catch(WatcherInterruptException &e) {
				Log::logger->log("ENCODER", DEBUG) << " Received EINTR in Encoder loop " << endl;
			}
		} while(this->running());
	} catch(WatcherErrorException &e) {
		Log::logger->log("ENCODER", ERROR) << "An error occurs, have to quit : " << e << endl;
		this->stop();
	} catch(EncoderStreamException &e) {
		Log::logger->log("ENCODER", ERROR) << "An error occurs, have to quit : EncoderStreamException" << endl;
		this->stop();
	}
	Log::logger->log("ENCODER", DEBUG) << "Encoder loop ended" << endl;	
}


void Encoder::readclear() throw (EncoderStreamException) {
	Log::logger->log("ENCODER", DEBUG) << "Enter readclear" << endl;
	char buffer[BUFFERSIZE];
	int count;
	//Child has write on its stdout
	count = this->clear->read(buffer, BUFFERSIZE-1);
	if (count > 0) {
		buffer[count]=0;
		Log::logger->log("ENCODER", DEBUG) << "Read: " << buffer<< endl;
		Message * out=new Message(buffer);
		try {
			out->send(this->encoded);
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
	count = this->encoded->read(buffer, BUFFERSIZE-1);
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
							if (this->clear!=NULL) {
								Message * msg=new Message(this->packet);
								msg->sendmsg(this->clear);
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



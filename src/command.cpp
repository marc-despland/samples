#include <iostream>
#include "command.h"
#include <string.h>
#include <stdio.h>
#define MAXCMDSIZE	1024
#define MAXCMDCODE	999
#define CMDPRECISION	3
#define CMDFORMAT	"%03u"

const char* InvalidCommandException::what() {
	return "Packet not a valid command";
}

const char* InvalidCmdCodeException::what() {
	return "The code for the command is invalid";
}


Command::~Command() {
}

Command::Command():Packet(Command::CODECOMMAND) {
	this->cmd=Command::NOCMD;
}


Command::Command(unsigned int cmd, const char * format, ...) throw(InvalidCmdCodeException):Packet(Command::CODECOMMAND) {
	if (cmd>MAXCMDCODE) {
		throw  InvalidCmdCodeException();
	}
	char * tmp=new char[MAXCMDSIZE+1];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(tmp, MAXCMDSIZE, format, argptr);
	va_end(argptr);
	this->cmd=cmd;
	this->length=(unsigned int) strlen(tmp)+CMDPRECISION;
	this->datasize=this->length;
	this->data=new char[this->datasize+1];
	this->data[this->datasize]=0;
	sprintf(this->data,CMDFORMAT,this->cmd);
	memcpy((void *) (this->data+CMDPRECISION), tmp, strlen(tmp));
}

Command::Command(Packet * packet) throw(InvalidCommandException, PacketNotReadyException, InvalidCmdCodeException):Packet(Command::CODECOMMAND){
	if (! Command::isCommand(packet)) 	throw  InvalidCommandException();
	if (! packet->isReady()) 		throw  PacketNotReadyException();
	if (packet->getLength()<CMDPRECISION) 	throw InvalidCmdCodeException();
	
	char tmp[CMDPRECISION+1];
	tmp[CMDPRECISION]=0;
	memcpy((void *) tmp, packet->getData(), CMDPRECISION);
	bool isnumber=true;
	for (int i=0;i<CMDPRECISION;i++) isnumber=isnumber && isdigit(tmp[i]);
	if (!isnumber) 				throw InvalidCmdCodeException();

	int result=sscanf(tmp,CMDFORMAT,&(this->cmd));
	if (result!=1) 				throw InvalidCmdCodeException();
	
	this->length=packet->getLength();
	this->datasize=packet->getLength();
	this->data=new char[this->datasize+1];
	this->data[this->datasize]=0;
	memcpy((void *) this->data, packet->getData(), this->datasize);	
}

unsigned int Command::command() {
	return this->cmd;
}

int Command::parameters(const char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	int result=vsscanf(this->data+CMDPRECISION, format, argptr);
	va_end(argptr);
	return result;
}

bool Command::isCommand(Packet * packet) {
	return ((packet->getCode()==Command::CODECOMMAND) && packet->isReady());
}


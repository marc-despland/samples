#include <iostream>
#include "command.h"
#include <string.h>
#include <stdio.h>
#define MAXCMDSIZE	1024

const char* InvalidCommandException::what() {
	return "Packet not a valid command";
}

const char* InvalidCmdCodeException::what() {
	return "The code for the command is invalid";
}

Command::Command():Packet(Command::CODECOMMAND) {
	this->cmd=Command::NOCMD;
}


Command::Command(unsigned int cmd, const char * format, ...) throw(InvalidCmdCodeException):Packet(Command::CODECOMMAND) {
	if (cmd>999) {
		throw  InvalidCmdCodeException();
	}
	char * tmp=new char[MAXCMDSIZE+1];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(tmp, MAXCMDSIZE, format, argptr);
	va_end(argptr);
	this->cmd=cmd;
	this->length=(unsigned int) strlen(tmp)+3;
	this->datasize=this->length;
	this->data=new char[this->datasize];
	sprintf(this->data,"%03u",this->cmd);
	memcpy((void *) (this->data+3), tmp, strlen(tmp));
}

Command::Command(Packet packet) throw(InvalidCommandException, PacketNotReadyException):Packet(Command::CODECOMMAND){
	if (! Command::isCommand(packet)) {
		throw  InvalidCommandException();
	} 
		
	if (! packet.isReady()) {
		throw  PacketNotReadyException();
	}
	this->length=packet.getLength();
	this->datasize=packet.getLength();
	this->data=new char[this->datasize+1];
	this->data[this->datasize]=0;
	memcpy((void *) this->data, packet.getData(), this->datasize);
}

unsigned int Command::command() {
	return this->cmd;
}

int Command::parameters(const char * format, ...) {
	return 0;
}

bool Command::isCommand(Packet packet) {
	return ((packet.getCode()==Command::CODECOMMAND) && packet.isReady());
}


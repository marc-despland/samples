#include "message.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

const char* InvalidMessageException::what() {
	return "Packet not a valid message";
}

Message::Message():Packet(Message::CODEMESSAGE) {
}

Message::Message(string str):Packet(Message::CODEMESSAGE) {
	this->length=(unsigned int) str.length();
        this->datasize=this->length;
        this->data=new char[this->datasize];
        memcpy((void *) this->data, (const void *)str.data(), this->datasize);
}


Message::Message(const char str[]):Packet(Message::CODEMESSAGE) {
        this->length=(unsigned int) strlen(str);
        this->datasize=this->length;
        this->data=new char[this->datasize];
        memcpy((void *) this->data, (const void *)str, this->datasize);
}

Message::Message(Packet packet) throw(InvalidMessageException):Packet(Message::CODEMESSAGE){
	if (! Message::isMessage(&packet)) {
		throw  InvalidMessageException();
	} else {
		
		this->length=packet.getLength();
		this->datasize=packet.getLength();
		this->data=new char[this->datasize+1];
		this->data[this->datasize]=0;
		memcpy((void *) this->data, packet.getData(), this->datasize);
	}
}

char * Message::message() throw (PacketNotReadyException){
	if (!this->isReady()) throw PacketNotReadyException();
	if (this->data==NULL) return "";
	char * tmp=new char[this->getLength()+1];
	tmp[this->getLength()]=0;
	memcpy((void *) tmp, this->data, this->datasize);
	return tmp;

}


long Message::sendmsg(int fd) throw (PacketNotReadyException){
	long size=0;
	if (!this->isReady()) throw PacketNotReadyException();
	if (this->data==NULL) return 0;
	size=write(fd,this->data,this->datasize);
	return size;
}

bool Message::isMessage(Packet * packet) {
	return ((packet->getCode()==Message::CODEMESSAGE) && packet->isReady());
}


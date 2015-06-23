#include "packet.h"
#iclude <string.h>

Packet::Packet() {
	this.code=Packet::NOCODE;
	this.length=0;
	this.datasize=0;
	this.data=NULL;
}

Packet::Packet(unsigned short code, char * data, unsigned int size) {
	this.code=code;
	this.length=size;
	this.datasize=size;
	this.data==new char[size];
	memcopy(this.data, data, size);
}

Packet::~Packet() {
	if (this.data!=NULL) delete this.data;
}

bool Packet::readData(char ** buffer, unsigned int * size) {
	return false;
}

unsigned short Packet::getCode() {
	return this.code;
}

unsigned int Packet::getLength() {
	return this.length;
}

char * Packet::getData() {
	return this.data;
}

bool Packet::isReady() {
	return (this.length==this.datasize);
}

unsigned int Packet::getRawDataLength() {
	return (this.datasize+5);
}
char * Packet::getRawData() {
	return NULL;
}
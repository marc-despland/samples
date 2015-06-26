#include "packet.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <unistd.h>

#define CODE_PRECISION		1
#define LENGTH_PRECISION	4
#define HEADERS_SIZE		CODE_PRECISION+LENGTH_PRECISION




const char* PacketInvalidHeaderException::what() {
	return "Can't decode packet headers";
}


const char* PacketNotReadyException::what() {
	return "Operation forbidden on packet not ready";
}


Packet::Packet(unsigned short code) {
	this->code=code;
	this->length=0;
	this->datasize=0;
	this->data=NULL;
}


Packet::Packet() {
	this->code=Packet::NOCODE;
	this->length=0;
	this->datasize=0;
	this->data=NULL;
}

Packet::Packet(unsigned short code, char * data, unsigned int size) {
	this->code=code;
	this->length=size;
	this->datasize=size;
	this->data=new char[size];
	memcpy(this->data, data, size);
}

Packet::~Packet() {
	if (this->data!=NULL) delete this->data;
}


bool Packet::readData(Buffer * buffer) throw (PacketInvalidHeaderException,PacketBufferSizeException) {
	if (this->code==Packet::NOCODE) {
		char * headers=buffer->read(HEADERS_SIZE);
		bool isnumber=true;
		for (int i=0;i<HEADERS_SIZE;i++) isnumber=isnumber && isdigit(headers[i]);
		if (!isnumber) {
			//We have an invalid packet
			throw PacketInvalidHeaderException();
		}
		int res=sscanf(headers,"%1hu%4u",&(this->code), &(this->length));
		delete headers;
		if (res!=2) {
			//We have an invalid packet
			throw PacketInvalidHeaderException();
		}
		this->data=new char[this->length+1];
		memset(this->data,0,this->length+1);
	}
	//So here we have a valid code and length
	unsigned int toread=this->length-this->datasize;
	if (toread>buffer->size()) toread=buffer->size();
	try {
		char * tmp=buffer->read(toread);
		memcpy(this->data+this->datasize, tmp, toread);
		this->datasize+=toread;
	} catch(PacketBufferSizeException &e) {
		//Impossible 
	}
	return (this->length==this->datasize);
}

unsigned short Packet::getCode() {
	return this->code;
}

unsigned int Packet::getLength() {
	return this->length;
}

char * Packet::getData() {
	return this->data;
}

bool Packet::isReady() {
	return (this->code!=Packet::NOCODE) && (this->length==this->datasize);
}

unsigned int Packet::getRawDataLength() {
	return (this->datasize+HEADERS_SIZE);
}
char * Packet::getRawData() {
	char * tmp=new char[this->length+HEADERS_SIZE+1];
	memset(tmp,0,this->length+HEADERS_SIZE+1);
	sprintf(tmp,"%1hu%04u",this->code, this->length);
	memcpy(tmp+HEADERS_SIZE, this->data, this->datasize);
	return tmp;
}

long Packet::send(int fd) throw (PacketNotReadyException){
	long size=0;
	if (!this->isReady()) throw PacketNotReadyException();
	if (this->data==NULL) return 0;
	char * tmp=new char[HEADERS_SIZE+1];
	memset(tmp,0,HEADERS_SIZE+1);
	sprintf(tmp,"%1hu%04u",this->code, this->length);
	size=write(fd, tmp, strlen(tmp));
	size+=write(fd,this->data,this->datasize);
	delete tmp;
	return size;
}

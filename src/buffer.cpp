#include "buffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

PacketBufferSizeException::PacketBufferSizeException(unsigned int size, unsigned int requested):exception(){
	this->size=size;
	this->requested=requested;
}	

const char* PacketBufferSizeException::what() {
	char * tmp =new char[40];
	memset(tmp,0,40);
	sprintf(tmp,"Buffer too small (%ud) to read %ud",size,requested);
	return tmp;
}


Buffer::Buffer() {
	this->datasize=0;
	this->allocated=0;
	this->data=NULL;
}

void Buffer::clear() {
	this->datasize=0;
	this->allocated=0;
	if (this->data!=NULL) free(this->data);
	this->data=NULL;
}


Buffer::~Buffer() {
	if (this->data!=NULL) free(this->data);
}

unsigned int Buffer::size() {
	return this->datasize;
}

char * Buffer::raw() {
	return this->data;
}


void Buffer::write(const char * data, unsigned int size) {
	if (size+1>this->allocated) this->data = (char *) realloc(this->data,sizeof(char)*(size+1));
	this->allocated=size+1;
	memcpy(this->data+this->datasize, data, size);
	this->datasize+=size;
}


char * Buffer::read(unsigned int size) throw(PacketBufferSizeException) {
	if (this->datasize<size) {
		throw PacketBufferSizeException(this->datasize, size);
	}
	char * tmp=new char[size+1];
	tmp[size]=0;
	memcpy(tmp, this->data, size);
	this->datasize-=size;
	memcpy(this->data, this->data+size, this->datasize);
	memset(this->data+this->datasize,0,this->allocated-this->datasize);
	return tmp;
}


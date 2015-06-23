#include "message.h"
#include <string.h>

Message::Message():Packet() {
	this->code=Message::CODEMESSAGE;
}

Message::Message(string str):Packet() {
        this->code=Message::CODEMESSAGE;
	this->length=(unsigned int) str.length();
        this->datasize=this->length;
        this->data=new char[this->datasize];
        memcpy((void *) this->data, (const void *)str.data(), this->datasize);
}


Message::Message(const char str[]):Packet() {
        this->code=Message::CODEMESSAGE;
        this->length=(unsigned int) strlen(str);
        this->datasize=this->length;
        this->data=new char[this->datasize];
        memcpy((void *) this->data, (const void *)str, this->datasize);
}


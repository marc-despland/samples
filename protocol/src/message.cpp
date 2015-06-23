#include "message.h"

Message::Message():Packet() {
	this.code=Message::CODEMESSAGE;
}
		
Message Message::operator=(string str) {
	this.length=str.length;
	this.datasize=str.length;
	this.data=new char[this.datasize];
	memcopy(this.data, str.data, this.datasize);
}

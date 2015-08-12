#include "host.h"

Host::Host(string name, unsigned int port) {
	this->name=name;
	this->myport=port;
}
Host::~Host() {
}

unsigned int Host::port() {
	return this->myport;
}


std::ostream & operator<<(std::ostream &os, const Host& h) {
	return os << h.name<<":"<<h.myport;
}

std::ostream & operator<<(std::ostream &os, const Host * h) {
	return os << h->name<<":"<<h->myport;
}

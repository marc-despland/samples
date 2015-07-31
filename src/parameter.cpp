#include "parameter.h"


Parameter::Parameter(string name, string description, bool mandatory):Variable(description, mandatory) {
	this->name=name;
}

string Parameter::getName() {
	return this->name;
}

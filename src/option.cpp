#include "option.h"
#include <string.h>
#include <stdlib.h>
 
Option::Option(char shortopt, char * longopt, char * description, bool hasvalue, bool mandatory) {
	this->shortoption=shortopt;
	this->longoption=strdup(longopt);
	this->description=strdup(description);
	this->hasvalue=hasvalue;
	this->mandatory=mandatory;
	this->isset=false;
	this->value=NULL;
}

bool Option::isSet() {
	return (this->isset);
}

int Option::intValue() {
	return atoi(this->value);
}
long Option::longValue() {
	return atol(this->value);
}
double Option::doubleValue() {
	return atof(this->value);
}
char * Option::stringValue() {
	return strdup(this->value);
}

char Option::getShortOption() {
	return this->shortoption;
}
char * Option::getLongOption() {
	return this->longoption;
}
char * Option::getDescription() {
	return this->description;
}
bool Option::hasValue() {
	return this->hasvalue;
}
bool Option::isMandatory() {
	return this->mandatory;
}

void Option::set(char * value) {
	this->isset=true;
	if (value!=NULL) {
		this->value=strdup(value);
	} else {
		if (this->value!=NULL) free(this->value);
		this->value=NULL;
	}
}

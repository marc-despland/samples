#ifndef _OPTIONS_H
#define _OPTIONS_H
#include "boolean.h"

#define MDE_OPTIONS_VERSION "1.0.0"

typedef enum {NONE, STRING, INT, LONG, DOUBLE} optionType;

typedef struct Option {
	char shortoption;
	char * longoption;
	char * description;
	BOOL hasvalue;
	BOOL mandatory;
	BOOL isset;
	void * target;
	optionType type;
} Option;


typedef void (*display_version_function)(char*);
 
int parse_options(int argc, char **argv, Option options[],int options_length, display_version_function display_version);
#endif

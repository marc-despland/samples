#ifndef _OPTIONS_H
#define _OPTIONS_H
using namespace std;
#include <exception>
#include <vector>
#include "option.h"

class OptionsStopException : public exception {
	const char* what();
};


class Options {
	public:
		Options();
		void add(Option * option);
		void parse(int argc, char **argv) throw(OptionsStopException);
	protected:
		virtual void version(char * program)=0;
		void help(char * program);
	protected:
		vector<Option *> options;

};

#endif

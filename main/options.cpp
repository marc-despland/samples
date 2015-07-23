#include "options.h"
#include <iostream>

class MyOptions : public Options {

	public:
		MyOptions():Options() {
			this->valuei=new Option('i', "int", "An example of int parameter", true, false);
			this->add(this->valuei);
			this->valuel=new Option('l', "long", "An example of long parameter", true, false);
			this->add(this->valuel);
			this->valued=new Option('d', "double", "An example of double parameter", true, false);
			this->add(this->valued);
			this->values=new Option('s', "string", "An example of string parameter", true, true);
			this->add(this->values);
			this->valuen=new Option('n', "none", "An example of parameter without value", false, false);
			this->add(this->valuen);
		}
		void version(char * program) {
			cout << "Test program of options decoder : " << program << " version 1.0" << endl;
		}
		Option * valuei;
		Option * valuel;
		Option * valued;
		Option * values;
		Option * valuen;
};

int main(int argc, char **argv) {
	MyOptions options;
	
	try {
		options.parse(argc, argv);
		cout << "Option successfully decoded" <<endl;
		if (options.valuei->isSet()) cout << "Value Int 	: " << options.valuei->intValue() << endl;
		if (options.valuel->isSet()) cout << "Value Long 	: " << options.valuel->longValue() << endl;
		if (options.valued->isSet()) cout << "Value Double 	: " << options.valued->doubleValue() << endl;
		if (options.values->isSet()) cout << "Value String 	: " << options.values->stringValue() << endl;
		if (options.valuen->isSet()) {
			cout << "Value None 	: is set" << endl;
		} else {
			cout << "Value None 	: is not set" << endl;
		}
		
	} catch (OptionsStopException e) {
		cout << "Can't execute, invalid option" << endl;
	}
}

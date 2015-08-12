#include "options.h"
#include <iostream>
#include "log.h"

int main(int argc, char **argv) {
	Options options(argv[0], "2.0.0", "Test program to check options parsing class");
	try {
		options.add('i', "int", "An example of int parameter", true, false);
		options.add('l', "long", "An example of long parameter", true, false);
		options.add('d', "double", "An example of double parameter", true, false);
		options.add('s', "string", "An example of string parameter", true, true);
		options.add('n', "none", "An example of parameter without value", false, false);
	} catch(ExistingOptionException &e ) {
	}
	try {
		options.parse(argc, argv);
		cout << "Option successfully decoded" <<endl;
		if (options.get('i')->isAssign()) cout << "Value Int\t: " << options.get("int")->asInt() << endl;
		if (options.get('l')->isAssign()) cout << "Value Long\t: " << options.get("long")->asInt() << endl;
		if (options.get('d')->isAssign()) cout << "Value Double\t: " << options.get("double")->asDouble() << endl;
		if (options.get('s')->isAssign()) cout << "Value String\t: " << options.get("string")->asChars() << endl;
		if (options.get('n')->isAssign()) {
			cout << "Value None\t: is set" << endl;
		} else {
			cout << "Value None\t: is not set" << endl;
		}
		
	} catch (OptionsStopException &e) {
		cout << "Can't execute, missing option" << endl;
	} catch (UnknownOptionException &e) {
		cout << "Unknown option requested" << endl;
	}
}

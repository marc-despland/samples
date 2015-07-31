#include <iostream>
#include "options.h"
#include "parameters.h"
#include "log.h"
#include <stdlib.h>

int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	Log::logger->log("GLOBAL", NOTICE) << "Program started " << endl;
	Options options(argv[0], "1.0.0", "Check reading parameters file");
	try {
		options.add('f', "config", "The configuration file to parse", true, true);
		options.add('c', "create", "Generate a sample config file", false, false);
	} catch(ExistingOptionException &e ) {
	}
	try {
		options.parse(argc, argv);
		Parameters * params=new Parameters(options.get("config")->asString());
		params->add("test1", "A test for a mandatory parameter", true);
		params->add("int", "An optional int", false);
		params->add("test2", "A test for a another mandatory parameter", true);
		if (options.get("create")->isAssign()) {
			try {
				params->create();
			} catch(CantCreateFileException &e) {
				Log::logger->log("GLOBAL", EMERGENCY) << "Can't create file " << options.get("config")->asString()<< endl;
			}
			exit(0);
		} 
		try {
			try {
				params->parse();
				try {
					cout << "Parameter test1 : \"" << params->get("test1")->asString() << "\"" << endl;
					cout << "Parameter test2 : \"" << params->get("test2")->asString() << "\"" << endl;
					if (params->get("int")->isAssign()) {
						cout << "Parameter int : \"" << params->get("int")->asInt() << "\"" << endl;
					} else {
						cout << "Parameter int : not set" << endl;
					}
				} catch (UnknownParameterNameException &e) {
					Log::logger->log("GLOBAL", EMERGENCY) << "Not defined parameter " << endl;
				}
			}catch(FileNotFoundException &e) {
				Log::logger->log("GLOBAL", EMERGENCY) << "Can't open file " << options.get("config")->asString()<< endl;
			}catch(InvalidConfigFileException &e) {
				Log::logger->log("GLOBAL", EMERGENCY) << "Invalid configuration file " << options.get("config")->asString()<< endl;
			}
		}catch(FileNotFoundException &e) {
			Log::logger->log("GLOBAL", EMERGENCY) << "Can't open file  " << options.get("config")->asString()<< endl;
		}
		
		
		
	} catch (OptionsStopException &e) {
	} catch (UnknownOptionException &e) {
		cout << "Unknown option requested" << endl;
	}
}

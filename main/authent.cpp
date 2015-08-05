#include "options.h"
#include "authentication.h"
#include "log.h"
#include <iostream>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	Log::logger->log("MAIN", DEBUG) << "Log Level DEBUG" << endl;
	Options options(argv[0], "1.0.0", "Test authentication class using PAM");
	try {
		options.add('l', "login", "The user login", true, true);
		options.add('p', "password", "The user password", true, true);
	} catch(ExistingOptionException &e ) {
	}
	Authentication *authent=new Authentication();
	try {
		options.parse(argc, argv);
		
		try {
			authent->authenticate(options.get("login")->asString(), options.get("password")->asString());
			Log::logger->log("MAIN", INFO) << "Authentification success" << endl;
		} catch (PamException &e) {
			Log::logger->log("MAIN", EMERGENCY) << "Can't comlete PAM authentification" << endl;
		} catch (AuthentFailureException &e) {
			Log::logger->log("MAIN", INFO) << "Invalid credentials" << endl;
		}	
	} catch (OptionsStopException &e) {
		Log::logger->log("MAIN", EMERGENCY) << "Missing mandatory option" << endl;
	} catch (UnknownOptionException &e) {
		Log::logger->log("MAIN", EMERGENCY) << "Request access to unknown option" << endl;
	}

};

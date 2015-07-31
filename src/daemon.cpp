//http://www.enderunix.org/docs/eng/daemon.php
#include "daemon.h"
#include "log.h"

Daemon::Daemon(string program, string version, string description) {
	this->logfile=NULL;
	this->options=new Options(program,version, description);
	try {
		this->options->add('u', "usermod", "Do not start the program as a Daemon", false, false);
		this->options->add('f', "config", "The configuration file to use", true, true);
		this->options->add('c', "create", "Create a sample config file and exit", false, false);
		this->options->add('a', "action", "start|stop", true, false);
	} catch(ExistingOptionException &e ) {
		Log::logger->log("DAEMON", EMERGENCY) << "Can't create one of the command line option"<< endl;
	}
	this->parameters=new Parameters();
	try {
		this->parameters->add("log", "The log file to use to send log", true);
		this->parameters->add("loglevel", "The loglevel to use", true);
		this->parameters->add("pid", "The file to use to store the pid of the daemon", true);
	} catch(ExistingParameterNameException &e ) {
		Log::logger->log("DAEMON", EMERGENCY) << "Can't create one of the file parameter"<< endl;
	}
}

void Daemon::start(int argc, char **argv) throw (OptionsStopException, UnknownOptionException, CantCreateFileException) {
	this->options->parse(argc, argv);
	this->parameters->setFileName(this->options->get("config")->asString());
	if (this->options->get("create")->isAssign()) {
		try {
			this->parameters->create();
		} catch(CantCreateFileException &e) {
			Log::logger->log("DAEMON", EMERGENCY) << "Can't create file " << this->options->get("config")->asString()<< endl;
		}
		throw OptionsStopException();
	}
	try {
		this->parameters->parse();
		try {
			this->logfile=new filebuf();
			if (this->logfile->open(this->parameters->get("log")->asChars(), std::ios::app)==NULL) {
				Log::logger->log("DAEMON", EMERGENCY) << "Can't create log file " << this->parameters->get("log")->asString()<< endl;
				throw CantCreateFileException();
			}
			Log::logger=new Log(this->logfile);
			Log::logger->setLevel(Log::levelStr(this->parameters->get("loglevel")->asString()));
			//So if everything is ok, here we have read all config parameter
			//If we are here, it means that an action is requested
			
			
			
			//We will be able to start to prepare to run the daemon
			if (this->options->get("create")->isAssign()) {
				Log::logger->log("DAEMON", NOTICE) << "program is start in user space (not as a daemon)"<< endl;
				//we will start in user mod (not as a daemon)
				this->daemon();
			} else {
				//we start as a daemon : we have to fork;
			}
			
		} catch (UnknownParameterNameException &e) {
			Log::logger->log("DAEMON", EMERGENCY) << "Not defined parameter " << endl;
		}
	}catch(FileNotFoundException &e) {
		Log::logger->log("DAEMON", EMERGENCY) << "Can't open file " << this->options->get("config")->asString()<< endl;
	}catch(InvalidConfigFileException &e) {
		Log::logger->log("DAEMON", EMERGENCY) << "Invalid configuration file " << this->options->get("config")->asString()<< endl;
	}
}

Daemon::~Daemon() {
	delete this->options;
	delete this->parameters;
	if (this->logfile!=NULL) this->logfile->close();
}

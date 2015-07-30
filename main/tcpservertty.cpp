#include <iostream>
#include "options.h"
#include "tcpserverterminal.h"
#include "log.h"

TcpServerTerminal * server;

class MyOptions : public Options {

	public:
		MyOptions():Options() {
			this->port=new Option('p', "port", "The port to listen on", true, true);
			this->add(this->port);
			this->pool=new Option('s', "pool", "The size of the connection pool", true, true);
			this->add(this->pool);
		}
		void version(char * program) {
			cout<< program << " : Test program of TcpServerTerminal class" << endl << "Version 1.0" << endl;
		}
		Option * port;
		Option * pool;
};

void received_SIGINT(int sig) {
	Log::logger->log("GLOBAL", INFO) << "Kill detected by TcpServerTTY" << endl;
	server->stop();
}

int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	Log::logger->log("GLOBAL", NOTICE) << "Program started " << endl;
	MyOptions options;
	server=new TcpServerTerminal();
	
	
	struct sigaction * eventInterrupted=new struct sigaction();
	sigemptyset(&(eventInterrupted->sa_mask));
	eventInterrupted->sa_flags=0;
	eventInterrupted->sa_handler= received_SIGINT;
	
	sigaction(SIGINT,eventInterrupted, NULL);
	
	try {
		options.parse(argc, argv);
		server->start(options.port->intValue(),options.pool->intValue());
		
	} catch (OptionsStopException &e) {
	} catch (TcpServerBindException &e) {
		Log::logger->log("GLOBAL", EMERGENCY) << "Can't listen on requested socket" << endl;
	}
	delete server;
}

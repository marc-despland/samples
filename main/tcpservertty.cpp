#include <iostream>
#include "options.h"
#include "tcpserverterminal.h"
#include "log.h"

TcpServerTerminal * server;


void received_SIGINT(int sig) {
	Log::logger->log("GLOBAL", INFO) << "Kill detected by TcpServerTTY" << endl;
	server->stop();
}

int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	Log::logger->log("GLOBAL", NOTICE) << "Program started " << endl;
	Options options(argv[0], "1.0.0", "Implement TcpServer class to communicate with TcpClient");
	try {
		options.add('s', "pool", "The size of the connection pool", true, true);
		options.add('p', "port", "The port to connect to", true, true);
	} catch(ExistingOptionException &e ) {
	}
	server=new TcpServerTerminal();
	
	
	struct sigaction * eventInterrupted=new struct sigaction();
	sigemptyset(&(eventInterrupted->sa_mask));
	eventInterrupted->sa_flags=0;
	eventInterrupted->sa_handler= received_SIGINT;
	
	sigaction(SIGINT,eventInterrupted, NULL);
	
	try {
		options.parse(argc, argv);
		server->start(options.get("port")->asInt(),options.get("pool")->asInt());
		
	} catch (OptionsStopException &e) {
	} catch (TcpServerBindException &e) {
		Log::logger->log("GLOBAL", EMERGENCY) << "Can't listen on requested socket" << endl;
	}catch (UnknownOptionException &e) {
		cout << " Request unknown option"<<endl;
	}
	delete server;
}

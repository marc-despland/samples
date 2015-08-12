#include <iostream>
#include "options.h"
#include "server.h"
#include "connectiontcp.h"
#include "clienthandlerterminal.h"
#include "log.h"

Server<ConnectionTCP> * server;


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
	
	struct sigaction * eventInterrupted=new struct sigaction();
	sigemptyset(&(eventInterrupted->sa_mask));
	eventInterrupted->sa_flags=0;
	eventInterrupted->sa_handler= received_SIGINT;
	
	sigaction(SIGINT,eventInterrupted, NULL);
	
	try {
		options.parse(argc, argv);
		Host * endpoint=new Host("0.0.0.0", options.get("port")->asInt());
		Log::logger->log("MAIN",NOTICE) << "Daemon will listen on " << endpoint<< endl;
		Server<ConnectionTCP> * server=new Server<ConnectionTCP>(endpoint, options.get("pool")->asInt(),new TerminalFactory());
		try {
			server->start();		
		} catch(ConnectionListenException &e) {
			Log::logger->log("MAIN", EMERGENCY) << "Can't listen on requested socket" << endl;
		}		
	} catch (OptionsStopException &e) {
	} catch (UnknownOptionException &e) {
		cout << " Request unknown option"<<endl;
	}
	delete server;
}

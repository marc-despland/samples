#include <iostream>
#include "options.h"
#include "tcpserverterminal.h"

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

int main(int argc, char **argv) {
	MyOptions options;
	TcpServerTerminal server;
	
	try {
		options.parse(argc, argv);
		server.start(options.port->intValue(),options.pool->intValue());
		
	} catch (OptionsStopException e) {
	}
}

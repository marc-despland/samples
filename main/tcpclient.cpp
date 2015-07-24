#include "options.h"
#include <iostream>
#include "tcpclient.h"

class MyOptions : public Options {

	public:
		MyOptions():Options() {
			this->server=new Option('s', "server", "The server IP or FQDN to connect to", true, true);
			this->add(this->server);
			this->port=new Option('p', "port", "The port to connect to", true, true);
			this->add(this->port);
		}
		void version(char * program) {
			cout<< program << " : Test program of TcpClient class" << endl << "Version 1.0" << endl;
		}
		Option * server;
		Option * port;
};


int main(int argc, char **argv) {
	MyOptions options;
	TcpClient client;
	
	try {
		options.parse(argc, argv);
		client.setClearFd(STDIN_FILENO, STDOUT_FILENO);
		client.start(options.server->stringValue(), options.port->intValue());
		
	} catch (OptionsStopException e) {
	}
}

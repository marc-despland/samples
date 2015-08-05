#include "options.h"
#include <iostream>
#include "tcpclient.h"
#include "log.h"

int main(int argc, char **argv) {
	Log::logger->setLevel(NONE);
	Options options(argv[0], "1.0.0", "Implement TcpClient class to communicate with TcpServer");
	try {
		options.add('s', "server", "The server IP or FQDN to connect to", true, true);
		options.add('p', "port", "The port to connect to", true, true);
		options.add('d', "debug", "Start client on debug mode", false, false);
	} catch(ExistingOptionException &e ) {
	}
	TcpClient * client=new TcpClient();
	
	try {
		options.parse(argc, argv);
		if (options.get('d')->isAssign()) Log::logger->setLevel(DEBUG);
		client->setClearFd(STDIN_FILENO, STDOUT_FILENO);
		client->start(options.get('s')->asChars(), options.get('p')->asInt());
		
	} catch (OptionsStopException &e) {
	} catch (CantConnectException &e) {
		cout << " Can't establish connect to the server"<<endl;
	}catch (UnknownOptionException &e) {
		cout << " Request unknown option"<<endl;
	}
	
	delete client;
}

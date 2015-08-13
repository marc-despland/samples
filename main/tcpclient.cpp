#include "options.h"
#include <iostream>
#include "clientencoder.h"
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
	ClientEncoder<ConnectionTCP> * client=ClientEncoder<ConnectionTCP>::client();
	try {
		options.parse(argc, argv);
		if (options.get('d')->isAssign()) Log::logger->setLevel(DEBUG);
		client->ioChannel(new Channel(STDIN_FILENO, STDOUT_FILENO));
		client->connect(new Host(options.get('s')->asChars(), options.get('p')->asInt()));
		
	} catch (OptionsStopException &e) {
	} catch (CantConnectException &e) {
		cout << " Can't establish connect to the server"<<endl;
	}catch (UnknownOptionException &e) {
		cout << " Request unknown option"<<endl;
	}
	
	delete client;
}

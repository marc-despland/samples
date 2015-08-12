
#include "server.h"
#include "log.h"
#include <unistd.h>

class MyHandler:public ClientHandler {
	public:	
		MyHandler(Connection *cnx):ClientHandler(cnx) {}
		void action() {
			Log::logger->log("MYHANDLER",DEBUG) << "Do server stuff on a connection" << endl;
			::sleep(5);
			Log::logger->log("MYHANDLER",DEBUG) << "Finish to do stuff" << endl;
		}

};

class Factory: public ClientHandlerFactory {
	public:
		MyHandler * createClientHandler(Connection * cnx) {
			return new MyHandler(cnx);
		}
};

int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	Host * endpoint=new Host("0.0.0.0", 6666);
	Server<ConnectionTCP> * server=new Server<ConnectionTCP>(endpoint, 10,new Factory());
	try {
		server->start();
			
	} catch(ConnectionListenException &e) {
		Log::logger->log("MAIN",EMERGENCY) << "Can't start listenning on host " << endpoint << " message: " << e << endl;
	}	
	return 0;
}
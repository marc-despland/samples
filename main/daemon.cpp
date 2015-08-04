#include "daemon.h"
#include "log.h"
#include "tcpserverterminal.h"

class TestDaemon:public Daemon {
	public:
		static void Initialize(string program, string version, string description) {
			if (Daemon::me==NULL) {
				Daemon::me=new TestDaemon(program, version, description);
			}
		}
		~TestDaemon() {
			delete this->server;
		}
	protected :
		TestDaemon(string program, string version, string description):Daemon(program, version, description) {
			try {
				this->parameters->add("port", "The listen port of the server", true, "8080");
				this->parameters->add("pool", "The size of the connection pool", true, "5");
			} catch(ExistingParameterNameException &e ) {
				Log::logger->log("MAIN", EMERGENCY) << "Can't create one of the file parameters"<< endl;
			}
			this->server=new TcpServerTerminal();
		}
		void daemon(){
			Log::logger->log("MAIN",NOTICE) << "Child daemon started" << endl;
			try {
				this->server->start(this->parameters->get("port")->asInt(),this->parameters->get("pool")->asInt());
			} catch (TcpServerBindException &e) {
				Log::logger->log("MAIN", EMERGENCY) << "Can't listen on requested socket" << endl;
			}
				
		}
		void terminate(){
			Log::logger->log("MAIN",NOTICE) << "Child daemon terminate" << endl;
			this->server->stop();
		}
		
		TcpServerTerminal * server;
};




int main(int argc, char **argv) {
	Log::logger->setLevel(DEBUG);
	TestDaemon::Initialize(argv[0], "1.0.0", "Test program for class Daemon");
	try {
		TestDaemon::Start(argc, argv);
	} catch(ForkException &e) {
		Log::logger->log("MAIN",NOTICE) << "ForkException occurs" << endl;
	} catch(OptionsStopException &e) {
		Log::logger->log("MAIN",NOTICE) << "OptionsStopException occurs" << endl;
	} catch(UnknownOptionException &e) {
		Log::logger->log("MAIN",NOTICE) << "UnknownException occurs" << endl;
	} catch(CantCreateFileException &e) {
		Log::logger->log("MAIN",NOTICE) << "CantCreateFileException occurs" << endl;
	}
}

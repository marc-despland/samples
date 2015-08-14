#include "client.h"
#include "log.h"

	
template <typename Cnx>   
Client<Cnx>::Client():Runnable() {
	Log::logger->log("CLIENT",DEBUG) << "Creating the client " << endl;
	this->cnx=NULL;
}

template <typename Cnx>   
Client<Cnx>::~Client() {
	Log::logger->log("CLIENT",DEBUG) << "Deleting the client " << endl;
}

template <typename Cnx>   
void Client<Cnx>::connect(Host * server)  throw(CantConnectException){
	this->cnx=new Cnx(server);
	this->cnx->connect();
	this->execute();

}

template <typename Cnx>   
void Client<Cnx>::established(Connection * cnx) {
	Log::logger->log("CLIENT",DEBUG) << "Connection Established " << cnx << endl;
}

		
template <typename Cnx>   
void Client<Cnx>::closed(Connection * cnx) {
	Log::logger->log("CLIENT",DEBUG) << "Connection closed " << cnx << endl;
}

template <typename Cnx>   
void Client<Cnx>::close() {
	Log::logger->log("CLIENT",DEBUG) << "Close connection" << endl;
	this->stop();
	this->cnx->shutdown();
}

template class Client<Connection>;

#ifdef HAVE_TCP_CONNECTION
template class Client<ConnectionTCP>;
#endif

#ifdef HAVE_RFCOMM_CONNECTION
template class Client<ConnectionRFCOMM>;
#endif


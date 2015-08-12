#ifdef HAVE_TCP_CONNECTION
#include "connectiontcp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include "log.h"
#include <errno.h>
#include <string.h>

ConnectionTCP::ConnectionTCP(Host * host):Connection(host){

}
ConnectionTCP::ConnectionTCP(int socket,Host * host):Connection(socket, host){

} 
ConnectionTCP::~ConnectionTCP(){
}
		
void ConnectionTCP::listen(unsigned int size) throw(ConnectionListenException){
	struct sockaddr_in server;
	this->socketfd=::socket(AF_INET,SOCK_STREAM,0);

	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(this->host->port());
	if (::bind(this->socketfd,(struct sockaddr *)&server,sizeof(server))<0) {
		Log::logger->log("CNXTCP", DEBUG) << "Failed to bind the socket " << this->socketfd << " try to bind to " << this->host<<endl;
		throw ConnectionListenException("Can't bind socket");
	}

	if (::listen(this->socketfd,size)<0) {
		Log::logger->log("CNXTCP", DEBUG) << "Failed to listen on socket " << this->socketfd << " bind to " << this->host<<endl;
		throw ConnectionListenException("Can't listen on socket");
	}
	this->Connection::listen(size);
}


ConnectionTCP * ConnectionTCP::accept() throw(ConnectionAcceptException) {
	Log::logger->log("CNXTCP", DEBUG) << "Wait for a new client" <<endl;
	int clientfd;
	struct sockaddr_in client;
	socklen_t clientsize;
	bzero(&client,sizeof(client));
	clientsize=sizeof(client);
	do {
		errno=0;
		bzero(&client,sizeof(client));
		clientsize=sizeof(client);
		clientfd = ::accept(this->socketfd,(struct sockaddr *)&client,&clientsize);
		Log::logger->log("CNXTCP",DEBUG) << "Accept socket: " << clientfd << " error : "<< strerror(errno) <<endl;
	} while ((clientfd<0) && (errno==EINTR));
	if (clientfd<=0) throw ConnectionAcceptException("Invalid socket return by accept");
	
	char address[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client.sin_addr), address, INET6_ADDRSTRLEN );
	int port=ntohs(client.sin_port);
	Log::logger->log("CNXTCP", DEBUG) << "We will return a new Connection"<<endl;
	return new ConnectionTCP(clientfd, new Host(address, port));;
}
void ConnectionTCP::shutdown() {
	::shutdown(this->socketfd, SHUT_RDWR);
	::close(this->socketfd);
	this->Connection::shutdown();
}



#endif
#ifndef _CONNECTIONTARGETTCP_H
#define _CONNECTIONTARGETTCP_H

class ConnectionTargetTcp: public ConnectionTarget {
	public:
		ConnectionTargetTcp(string host, unsigned int port) throw(UnknownHostException, UnkownConnectionTypeException);
		~ConnectionTargetTcp();
		
		struct sockaddr_in * target();
	protected:
		struct sockaddr_in * endpoint;
	

};

#endif
#ifndef _TCPCONNECTION_LISTENER_H
#define _TCPCONNECTION_LISTENER_H

class TcpConnectionListener {
	public:
		virtual void connectionClosed(int socket)=0;
};

#endif

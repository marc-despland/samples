#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H
#include <exception>
using namespace std;
#include "client.h"

class CantConnectException : public exception {
	public :
		CantConnectException(int error) {
			this->error=error;
		}
	int error;
	const char* what();
};
class TcpClient:public Client {
	public:
		TcpClient();
		~TcpClient();
	 	void start(char * server, int port) throw(CantConnectException);
	protected:
		char * server;
		int port;
		int serverfd;
};
#endif

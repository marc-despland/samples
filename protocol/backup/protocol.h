#ifndef _PROTOCOL_SERVER_H
#define _PROTOCOL_SERVER_H
#include "iprotocol.h"
using namespace std;
#define	MDE_PROTOCOL_VERSION	"1.0.0"


class Protocol: public IProtocol {
	public:
		Protocol();
		~Protocol();
		void setAction(IProtocolAction * action);
		char * decodeBuffer(char * buffer, int size);
		char * encodeMessage(char * message, int * size);
		char * encodeAction(unsigned short code, char * argv, int * size);
		bool isDecoderReady();
		
	protected:
		IProtocolAction * action;
};
#endif
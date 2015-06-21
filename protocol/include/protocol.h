#ifndef _PROTOCOL_SERVER_H
#define _PROTOCOL_SERVER_H
#include "iprotocol.h"
using namespace std;
#define	MDE_PROTOCOL_VERSION	"1.0.0"

typedef void (*protocol_resizetty)(unsigned short row, unsigned short col);

void setResizeTTYHandler(protocol_resizetty resizetty);

/*
Initialize the context of the protocol decoder
THIS FUNCTION HAS TO BE CALL FIRST
*/
void init() ;

/*
Return true if something is ready to decode
*/ 
boolean isready() {

char * protocol_decoder(char * buffer, int buffersize);
char * protocol_encoder(char * message, int * buffersize);


class Protocol: public IProtocol {
	public:
		static setAction(IProtocolAction * action);
		static char * decoder(char * buffer, int size);
		static char * encoder(char * message, int * size);
		static bool isready();
	protected:
		Protocol();
		~Protocol();
		static Protocol * protocol;
		
	protected:
		IProtocolAction * action;
};
#endif
#ifndef _IPROTOCOL_H
#define _IPROTOCOL_H

class IProtocol {
	public:
		virtual static char * decoder(char * buffer, int size);
		virtual static char * encoder(char * message, int * size);
}

#endif;
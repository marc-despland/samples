#ifndef _IPROTOCOL_H
#define _IPROTOCOL_H

class IProtocol {
	public:
		virtual char * decoder(char * buffer, int size);
		virtual char * encoder(char * message, int * size);
}

#endif;
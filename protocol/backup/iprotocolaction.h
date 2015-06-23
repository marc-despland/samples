#ifndef _IPROTOCOLACTION_H
#define _IPROTOCOLACTION_H

class IProtocolAction {
	public:
		virtual message(char * message, int size);
		virtual command(int cmd, char * argv, int * size);
}

#endif;
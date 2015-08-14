#ifndef _CHANNEL_H
#define _CHANNEL_H

#include "watcher.h"
#include <termios.h>
#include <unistd.h>

class Watcher;
typedef struct {
	unsigned int w;
	unsigned int h;
} screensize;

class Channel {
	friend class Watcher;
	public:
		Channel(int input, int output);
		virtual ~Channel();
		
		void modePassword();
		void modeNormal();
		void modeTerminal();
		
		screensize window();
		void window(unsigned int w, unsigned int h);
		
		
		int read(const char * buffer, int size);
		int iread(const char * buffer, int size);
		int write(const char * buffer, int size);
		
	protected:
		int input;
		int output;
		struct termios normal;
};
#endif
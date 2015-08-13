#ifndef _WATCHER_H
#define _WATCHER_H
#include "channel.h"
#include <vector>
#include <map>
#include <iostream>
#include <exception>
#include <string>

using namespace std;

class WatcherErrorException: public exception {
	public:
		WatcherErrorException(string message):exception() {
			this->message=message;
		}
		~WatcherErrorException() throw (){};
		friend std::ostream & operator<<(std::ostream &os, const WatcherErrorException& e) {
			return os << e.message;
		}
	protected:
		string message;
};

class WatcherInterruptException : public exception {};

class Channel;
class Watcher {
	public:
		Watcher();
		virtual ~Watcher();
		void watch(Channel * channel);
		
		vector<Channel*> select() throw(WatcherInterruptException, WatcherErrorException);	
	protected:
		map<int, Channel *> watchlist;

};

#endif
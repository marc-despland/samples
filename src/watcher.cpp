#include "watcher.h"
#include "log.h"
#include <errno.h>
#include <string.h>
#include <sys/select.h>

       /* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>


Watcher::Watcher() {
}
Watcher::~Watcher() {
}

void Watcher::watch(Channel * channel) {
	this->watchlist[channel->input]=channel;
}
		
vector<Channel*> Watcher::select() throw(WatcherInterruptException, WatcherErrorException) {
	fd_set readset;
	int result;
	sigset_t emptyset;
	FD_ZERO(&readset);
	int maxfd=0;
	vector<Channel *> readyChannels;
	for (map<int, Channel *>::iterator it=this->watchlist.begin(); it!=this->watchlist.end(); ++it) {
		FD_SET(it->first, &readset);
		if (it->first>maxfd) maxfd=it->first;
	}
	result = pselect(maxfd+1, &readset, NULL, NULL, NULL, &emptyset);
	if (result>0) {
		for (map<int, Channel *>::iterator it=this->watchlist.begin(); it!=this->watchlist.end(); ++it) {
			if (FD_ISSET(it->first, &readset)) {
				readyChannels.push_back(it->second);
			}
		}
	} else {
		if (errno==EINTR) {
			throw WatcherInterruptException();
		} else {
			throw WatcherErrorException(::strerror(errno));
		}
	}
	return readyChannels;
}

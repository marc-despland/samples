#include "log.h"
#include <sys/types.h>
#include <unistd.h>



ostream onull(0);

Log * Log::logger=new Log(cout);

Log Log::cnull(onull);

Log::Log(ostream &out):ostream(out.rdbuf()) {
	this->level=NONE;
}

void Log::setLevel(LogLevel level) {
	this->level=level;
	for (std::map<string, LogLevel>::iterator it=this->groups.begin(); it!=this->groups.end(); ++it) {
		this->groups[it->first]=level;
	}
}

void Log::setLevel(string group,LogLevel level) {
	this->groups[group]=level;
}

string Log::strLevel(LogLevel level) {
	string str;
	switch (level) {
		case EMERGENCY:
			str="EMERG";
			break;
		case ALERT:
			str="ALERT";
			break;
		case CRITICAL:
			str="CRITICAL";
			break;
		case ERROR:
			str="ERROR";
			break;
		case WARNING:
			str="WARNING";
			break;
		case NOTICE:
			str="NOTICE";
			break;
		case INFO:
			str="INFO";
			break;
		case DEBUG:
			str="DEBUG";
			break;
		default:
			str="NONE";
	}
	return str;

}

Log &Log::log(LogLevel level) {
	if (this->level>=level) {
		*this << getpid() << " \t\t " << Log::strLevel(level) <<"\t ";
		return *this;
	}
	return cnull;
}


Log &Log::log(string group,LogLevel level) {
	LogLevel threshold=this->level;
	if (this->groups.count(group)==1) {
		threshold=this->groups[group];
	}
	if (threshold>=level) {
		*this << getpid() << " " << group <<"\t "<< Log::strLevel(level) <<"\t ";
		return *this;
	}
	return cnull;
}


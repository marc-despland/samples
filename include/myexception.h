#ifndef _MYEXCEPTION_H
#define _MYEXCEPTION_H
#include <exception>
#include <string>
using namespace std;

class MyException :public exception {
	protected :
		string target;
		
	public:
		MyException(string target);
		MyException();
		virtual const char * what();
};


#endif

#ifndef _RUNNABLE_H
#define _RUNNABLE_H
#include "irunnable.h"
using namespace std;

class Runnable : public IRunnable{
	public:
		Runnable();
		void start();
		void stop();
		bool running();
	protected:
		bool flag;

};
#endif

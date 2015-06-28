#ifndef _RUNNABLE_H
#define _RUNNABLE_H

using namespace std;

class Runnable {
	public:
		Runnable();
		void start();
		void stop();
		bool running();
	protected:
		bool flag;

};
#endif

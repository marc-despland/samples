#include "runnable.h"
#include <string>
#include <iostream>


class run1 : virtual public Runnable {
	public:
		run1():Runnable() {};
		virtual ~run1() {};
		virtual void status() {
			cout << "Status run1 : " << this->running() << endl;
			
		}
};
class run2 : virtual public Runnable {
	public:
		run2():Runnable() {};
		virtual ~run2() {};
		virtual void status() {
			cout << "Status run2 : " << this->running() << endl;
			
		}
};

class run3 : public run1, public run2 {
	public:
		run3():run1(),run2() {};
		virtual ~run3() {};
		virtual void status() {
			cout << "Status run1 : " << this->run1::running() << endl;
			cout << "Status run2 : " << this->run2::running() << endl;
			cout << "-----------------------------------------" << endl;
			this->run1::status();
			this->run2::status();
		}
		virtual void start() {
			this->Runnable::start();
		}
};


int main(int argc, char **argv) {
	run3 * run=new run3();
	run->status();
	run->start();
	run->status();


}
#include "termtty.h"
#include "fork.h"
#include "client.h"
#include <iostream>

class Sample:public Fork {
	public:
		Sample();

	protected:
		int c2t[2], t2c[2];
		TermTTY * terminal;
		Client * client;
		void child();
		void parent();
};

Sample::Sample() {
	pipe(this->c2t);
	pipe(this->t2c);
	cout << "TermTTY : " << this->c2t[1] << ", " << this->t2c[0]<< endl;
	cout << "Client : " << this->t2c[1] << ", " << this->c2t[0]<< endl;
	
	this->terminal=new TermTTY(this->c2t[0],this->t2c[1]);
	this->client=new Client(STDIN_FILENO, STDOUT_FILENO,this->t2c[0], this->c2t[1]);
}

void Sample::child() {
	cout << "start Sample child " << endl;
	this->client->run();
	cout << "stop Sample child " << endl;
}

void Sample::parent() {
	cout << "start Sample parent " << endl;
	this->terminal->terminal();
	cout << "stop Sample parent " << endl;

}

int main(int argc, char* argv[]) {
	Sample * sample=new Sample();
	sample->execute();
}

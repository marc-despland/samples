#include <iostream>
#include <string>

using namespace std;
#include "message.h"

int main(int argc, char* argv[]) {
	string str="test 2";
	Message msg="ceci est un test";
	Message msg2=str;
	cout << msg.getLength() << endl;
	
	return 0;
}

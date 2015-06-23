#include <iostream>
using namespace std;
#include "message.h"

int main(int argc, char* argv[]) {
	Message msg="ceci est un test";
	return cout << msg.getLength() << endl;
	
	return 0;
}
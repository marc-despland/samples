#ifndef _CONNECTIONFACTORY_H
#define _CONNECTIONFACTORY_H
using namespace std;


class ConnectionFactory {
	public:
		ConnectionFactory();
		virtual ~ConnectionFactory();
		int register(string type) throw(ConnectionTypeAlreadyExistsException);
		int type(string name) throw(UnkownConnectionTypeException); 
		
	protected:
		map<string, unsigned int>	registered;

};

#endif
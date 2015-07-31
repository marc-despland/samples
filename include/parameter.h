#ifndef _PARAMETER_H
#define _PARAMETER_H
#include "variable.h"

class Parameter: public Variable {
	public :
		Parameter(string name, string description, bool mandatory);
		string getName();
		
	protected :
		string name;
};

#endif

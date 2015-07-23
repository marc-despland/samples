#ifndef _OPTION_H
#define _OPTION_H

class Option {
	public:
		Option(char shortopt, char * longopt, char * description, bool hasvalue, bool mandatory);
		bool isSet();
		int intValue();
		double doubleValue();
		long longValue();
		char * stringValue();
		
		char getShortOption();
		char * getLongOption();
		char * getDescription();
		bool hasValue();
		bool isMandatory();
		void set(char * value);
	
	protected:
		char shortoption;
		char * longoption;
		char * description;
		bool hasvalue;
		bool mandatory;
		bool isset;
		char * value;

};

#endif

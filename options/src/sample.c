#include "options.h"


void version(char * cmd) {
	printf("Sample Command Line Option Parser version %s\n",MDE_OPTIONS_VERSION);
	printf("https://github.com/marc-despland/samples\n\n");
}
	char shortoption;
	char * longoption;
	char * description;
	BOOL hasvalue;
	BOOL mandatory;
	BOOL isset;
	void * target;
	optionType type;

int main(int argc, char **argv) {
	int valuei=0;
	long valuel=0;
	double valued=0;
	char * values="";
	Option options[]={
			{"i","int","An example of int parameter", TRUE,FALSE,FALSE,&valuei, INT},
			{"l","long","An example of long parameter", TRUE,FALSE,FALSE,&valuel, LONG},
			{"d","double","An example of double parameter", TRUE,FALSE,FALSE,&valued, DOUBLE},
			{"s","string","An example of string parameter", TRUE,FALSE,FALSE,&valuec, STRING},
			{"m","mandatory","A mandatory parameter", FALSE,TRUE,FALSE,NULL, NONE}	
	};
	
	int result= parse_options(argc, argv, options, version);
	printf("Result : %d\n",result);
	printf("	valuei %d\n",valuei);
	printf("	valuel %d\n",valuel);
	printf("	valued %f\n",valued);
	printf("	valuec %s\n",values);
	
	return 1;
}

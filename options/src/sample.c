#include <unistd.h>
#include <stdio.h>
#include "options.h"


void version(char * cmd) {
	printf("Sample Command Line Option Parser version %s\n",MDE_OPTIONS_VERSION);
	printf("https://github.com/marc-despland/samples\n\n");
}

int main(int argc, char **argv) {
	int valuei=0;
	long valuel=0;
	double valued=0;
	char * values="";
	Option options[]={
			{'i',"int","An example of int parameter", TRUE,FALSE,FALSE,&valuei, INT},
			{'l',"long","An example of long parameter", TRUE,FALSE,FALSE,&valuel, LONG},
			{'d',"double","An example of double parameter", TRUE,FALSE,FALSE,&valued, DOUBLE},
			{'s',"string","An example of string parameter", TRUE,FALSE,FALSE,&values, STRING},
			{'m',"mandatory","A mandatory parameter", FALSE,TRUE,FALSE,NULL, NONE}	
	};
	int i;

	int result= parse_options(argc, argv, options, 5, version);
	
	printf("\nResult : %d\n",result);
	printf("	valuei %d\n",valuei);
	printf("	valuel %ld\n",valuel);
	printf("	valued %f\n",valued);
	printf("	valuec %s\n",values);

	printf("\n");

	if (result>=0) {
		printf("Non option parameters :\n");
		for(i=result;i<argc;i++) printf("%d : %s\n",i,argv[i]);
	}
	return 1;
}

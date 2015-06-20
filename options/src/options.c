#include <unistd.h>
#include <string.h>
#include "options.h"

int parse_options(int argc, char **argv, Option options[], display_version_function display_version) {
	char * shortlist=(char *) malloc(sizeof(char)*((options.length+2)*2));
	struct option *longlist= (struct option *) malloc(sizeof(struct option)*(options.length+2));
	int index=0;
	shortlist[0]=0;
	int result=1;
	
	for (int i=0; i<options.length; i++) {
		options[i].isset=FALSE;
		strcat(shortlist,options[i].shortoption);
		longlist[i].name=strdup(options[i].longoption);
		if (options[i].hasvalue) {
			strcat(shortlist,":");
			longlist[i].has_arg=required_argument;
		} else {
			longlist[i].has_arg=no_argument;
		}
		longlist[i].flag=0;
		longlist[i].val=options[i].shortoption;
	}
	strcat(shortlist,"hv");
	longlist[options.length].name="help";
	longlist[options.length].has_arg=no_argument;
	longlist[options.length].flag=0;
	longlist[options.length].val='h';
	longlist[options.length+1].name="version";
	longlist[options.length+1].has_arg=no_argument;
	longlist[options.length+1].flag=0;
	longlist[options.length+1].val='v';
	
	while ((opt=getopt_long(argc, argv, shortlist,longlist, &index))>0) {
		if (longlist[index].val=='h') {
			//Display help message
			int count=2;
			printf(" usage : %s	[-h|--help] [-v|--version] ",argv[0]);
			for (int i=0; i<options.length; i++) {
				if (options[i].mandatory) {
					if (options[i].hasvalue) {
						printf("-%s|--%s value",options[i].shortoption, options[i].longoption);
					} else {
						printf("-%s|--%s ",options[i].shortoption, options[i].longoption);
					}
				} else {
					if (options[i].hasvalue) {
						printf("[-%s|--%s value]",options[i].shortoption, options[i].longoption);
					} else {
						printf("[-%s|--%s] ",options[i].shortoption, options[i].longoption);
					}
				}
				count++;
				if (count>5) {
					printf("\n			");
					count=0;
				}
			}
			printf("\n\nDescription:\n");
			printf("-h --help		: Display this help message\n");
			printf("-v --version	: Display %s version\n", argv[0]);
			for (int i=0; i<options.length; i++) {
				printf("-%s --%s	: %s\n",options[i].shortoption, options[i].longoption, options[i].description);
			}
			return(0);
		}
		if (longlist[index].val=='v') {
			//Display the version
			display_version(argv[0]);
			return(0);
		}
		if (index<options.index) {
			if (options[index].hasvalue) {
				if (optarg==0) {
					result=-1;
					printf("Error, missing value for -%s --%s : %s\n",options[index].shortoption, options[index].longoption, options[index].description);
				} else {
					options[index].isset=TRUE;
					switch (options[index].type) {
						case STRING:
							*(options[index].target)=strdup(optarg);
						break;
						case INT :
							*(options[index].target)=atoi(optarg);
						break;
						case LONG :
							*(options[index].target)=atol(optarg);
						break;
						case DOUBLE:
							*(options[index].target)=atof(optarg);
						break;
					}
				}
			} else {
				options[index].isset=TRUE;
			}
		}
	} //End while loop on getopt
	
	//Now check that every required option is set.
	for (int i=0; i<options.length; i++) {
		if (options[i].mandatory) {
			if (!options[i].isset) {
				result=-1;
				printf("Missing required parameter -%s --%s : %s\n",options[i].shortoption, options[i].longoption, options[i].description);
			}
		}
	}
	return result;
} 


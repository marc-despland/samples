#include "options.h"
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Options::Options() {
	this->options.push_back(new Option('h', "help", "Display this help message", false, false));
	this->options.push_back(new Option('v', "version", "Display software version", false, false));
}


void Options::add(Option * option) {
	this->options.push_back(option);
}

void Options::help(char * program) {
	int count=0;
	printf(" usage : %s	",program);
	for (unsigned int i=0; i<this->options.size(); i++) {
		if (this->options[i]->isMandatory()) {
			if (this->options[i]->hasValue()) {
				printf("-%c|--%s <value> ",this->options[i]->getShortOption(), this->options[i]->getLongOption());
			} else {
				printf("-%c|--%s ",this->options[i]->getShortOption(), this->options[i]->getLongOption());
			}
		} else {
			if (this->options[i]->hasValue()) {
				printf("[-%c|--%s <value>] ",this->options[i]->getShortOption(), this->options[i]->getLongOption());
			} else {
				printf("[-%c|--%s] ",this->options[i]->getShortOption(), this->options[i]->getLongOption());
			}
		}
		count++;
		if (count>3) {
			printf("\n			");
			count=0;
		}
	}
	printf("\n\nDescription:\n");
	for (unsigned int i=0; i<this->options.size(); i++) {
		printf("-%c --%s	: %s\n",this->options[i]->getShortOption(), this->options[i]->getLongOption(), this->options[i]->getDescription());
	}

}


void Options::parse(int argc, char **argv) throw(OptionsStopException) {
	char * shortlist=(char *) malloc(sizeof(char)*((this->options.size())*2));
	struct option *longlist= (struct option *) malloc(sizeof(struct option)*(this->options.size()));
	int index=0;
	int opt;
	shortlist[0]=0;
	bool error=false;
	bzero(shortlist, sizeof(char)*((this->options.size())*2));	

	//first we create the parameter option for getopt
	for (unsigned int i=0; i<this->options.size(); i++) {
		shortlist[strlen(shortlist)]=this->options[i]->getShortOption();
		longlist[i].name=strdup(this->options[i]->getLongOption());
		if (this->options[i]->hasValue()) {
			strcat(shortlist,":");
			longlist[i].has_arg=required_argument;
		} else {
			longlist[i].has_arg=no_argument;
		}
		longlist[i].flag=0;
		longlist[i].val=this->options[i]->getShortOption();
	}


	//then we parse the command line
	
	while ((opt=getopt_long(argc, argv, shortlist,longlist, &index))>0) {
		//printf("Read option:%c index=%d\n",opt,index);
		switch(opt) {
			case 'h':
				//Display help message
				this->help(argv[0]);
				throw OptionsStopException();
			break;
			case 'v':
				//Display the version
				this->version(argv[0]);
				throw OptionsStopException();
			break;
			case '?':
				//@printf("Unknown option\n");
			break;
			default:
				index=this->options.size();
				for (unsigned int i=0;i<this->options.size();i++) if (this->options[i]->getShortOption()==opt) index=i;
				if (index<this->options.size()) {
					if (this->options[index]->hasValue()) {
						if (optarg==0) {
							error=true;
							printf("Error, missing value for -%c --%s : %s\n",this->options[index]->getShortOption(), this->options[index]->getLongOption(), this->options[index]->getDescription());
						} else {
							this->options[index]->set(optarg);
						}
					} else {
						this->options[index]->set(NULL);
					}
				}
			break;
		}
	} //End while loop on getopt
	//Now check that every required option is set.
	for (unsigned int i=0; i<this->options.size(); i++) {
		if (this->options[i]->isMandatory()) {
			if (!this->options[i]->isSet()) {
				error=true;
				printf("Missing required parameter -%c --%s : %s\n",this->options[i]->getShortOption(), this->options[i]->getLongOption(), this->options[i]->getDescription());
			}
		}
	}
	if (error) {
		this->help(argv[0]);
		throw OptionsStopException();
	}
}

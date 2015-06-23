#include <errno.h>
#include <strings.h>
#incluse "boolean.h"
#include "protocol.h"

#define HEADER_UNSET	-1
#define HEADER_MESSAGE	1
#define HEADER_CMD		0


#define COMMAND_RESIZETTY	1


Protocol::Protocol() {
}

Protocol::~Protocol() {
}

void Protocol::setAction(IProtocolAction * action) {
	this->action=action;
}
char * Protocol::decodeBuffer(char * buffer, int size) {

}

char * Protocol::encodeMessage(char * message, int * size) {

}


char * Protocol::encodeAction(unsigned short code, char * argv, int * size) {

}


bool Protocol::isDecoderReady() {

}



typedef struct ProtocolContext {
	char * 				buffer;
	int 				buffersize;
	char				code;
	int					length;
	protocol_resizetty 	resizetty;
} ProtocolContext;

ProtocolContext context;

void setResizeTTYHandler(protocol_resizetty resizetty) {
	context.resizetty=resizetty;
}


void init() {
	context.buffer=NULL;
	context.buffersize=0;
	context.code=HEADER_UNSET;
	context.length=0;
	context.resizetty=NULL;
}

void adddata(char * buffer, int buffersize){
	char hstrsize[5];
	bzero(hstrsize,5);
	context.buffer=realloc(context.buffer, context.buffersize+buffersize);
	context.buffersize+=buffersize;
	if (context.code==HEADER_UNSET) {
		if (context.buffersize>5) {
			context.code=context.buffer[0];
			bcopy(context.buffer+1, hstrsize, 4);
			sscanf(hstrsize,"%04d",&(context.length));
			context.buffersize-=5;
			bcopy(context.buffer+5, hstrsize, context.buffersize);
		}
	}
}

BOOL message_ready() {
	return (context.code==HEADER_MESSAGE && context.buffersize>=context.length);
}
BOOL command_ready() {
	return (context.code==HEADER_CMD && context.buffersize>=context.length);
}

BOOL isready() {
	return (message_ready() || command_ready());
}

char * read_message(int * msgsize) {
	char * msg=NULL;
	if (message_ready()) {
		msg=(char *) malloc(sizeof(char)*(context->length+1));
		bzero(msg, context->length+1);
		bcopy(context->buffer, msg, context->length);
		context.code=HEADER_UNSET;
		bcopy(context->buffer+context->length, context->buffer, context->buffersize-context->length);
		context->length=0;
		context->buffersize-=context->length;
	}
	*msgsize=strlen(msg);
	return msg;
}

char * read_cmd(int * cmd; int * argvsize) {
	char * argv=NULL;
	char code[4];
	if (command_ready()) {
		bzero(code,4);
		bcopy(context.buffer, code, 3);
		sscanf(code,"%03d",cmd);
		argv=(char *) malloc(sizeof(char)*(context->length-2));
		bzero(argv, context->length-2);
		bcopy(context->buffer+3, argv, context->length-3);
		*argvsize=context->length-3;
		context.code=HEADER_UNSET;
		bcopy(context->buffer+context->length, context->buffer, context->buffersize-context->length);
		context->length=0;
		context->buffersize-=context->length;
	}
	return argv;
}


void execute_command(int cmd, char * argv, int argvsize) {
	switch(cmd) {
		case COMMAND_RESIZETTY :
			unsigned short row, col;
			sscanf(argv,"%dx%d", &row, &col);
			if (context.resizetty!=NULL) {
				context.resizetty(row,col);
			}
		break;
	}
}

char * protocol_decoder(char * buffer, int buffersize) {
	char * msg;
	int msglength=0;
	
	msg=(char *) malloc(sizeof(char)*(buffersize+1));
	bcopy(buffer,msg,buffersize);
	swicth(buffer[0]) {
		case	HEADER_CMD:
		break;
		case	HEADER_MESSAGE:
		break;
		default:
			printf("Received unknown message code : %c\n",buffer[0]);
		break;
	}
	msg[buffersize]=0;
	return msg;
}

char * protocol_encoder(char * msg, int * buffersize) {
	char * buffer;
	char * tmp;
	buffer=(char *) malloc(sizeof(char)*(strlen(msg)+6));
	buffer[0]=HEADER_MESSAGE;//we send a text message
	tmp=buffer+1;
	sprintf(tmp,"%04d%s",strlen(msg),msg);
	*buffersize=strlen(msg)+5;
	return buffer;
}

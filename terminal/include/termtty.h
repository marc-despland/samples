#ifndef _TERMINAL_SERVER_H
#define _TERMINAL_SERVER_H

#define	MDE_TERMINAL_VERSION	"1.2.0"

//Transform the data received to a string to send to the terminal input
typedef char * (*terminal_decoder)(char *, int);
//Transform the string output by the terminal to data to send
typedef char * (*terminal_encoder)(char *, int *);

void set_decoder(terminal_decoder decoder);
void set_encoder(terminal encoder encoder);

/*
Set the size of the TTY file descriptor
- row : The number of row of the terminal
- col : The number of colonnes of the terminal
*/
void resize_terminal(unsigned short row, unsigned short col);

/*
Start a terminal on a fork pty that execute a bash --login
input : the input file descriptor to send command to the child process
output : the ouput filedescriptor to display all child output
*/
int start_terminal(int input, int output);

#endif
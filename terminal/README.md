# Terminal

## Description 
Terminal implement a forkpty to start a bash --login that receive input from a given file descriptor and write output to another.
* src/termtty.c : implement the function of the terminal
* include/termtty.h : the header
* src/sample.c : an exemple using termtty to implement a terminal

## How to use
To implement a terminal you just have to call the function listent_request

```int start_terminal(int input, int output);```

* input : the input file descriptor
* output : the output file descriptor

## Build
``` make ```

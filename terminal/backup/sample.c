#include <unistd.h>
#include <sys/ioctl.h>
#include "termtty.h"


int main() {
	start_terminal(STDIN_FILENO, STDOUT_FILENO);
    return 1;
}

#include <stdio.h>
#include "brainfuck.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		perror("a.out filename");
		return -1;
	}
	run(argv[1]);
	return 0;
}

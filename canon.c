#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "minrep.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char** argv) {
	if (argc != 2) {
		fprintf(stderr, "An argument must be provided: string of letters or digits\n");
		return EXIT_FAILURE;
	}
	unsigned int d;
	size_t n = strlen(argv[1]);
	char *str = malloc(sizeof(char) * (n + 1));
	minrep(str, argv[1], n);
	str[n] = '\0';
	unsigned int msp = MSP(n, argv[1], &d);
	fprintf(stdout, "%d\t%d\t%s\n", msp + 1, d, str);
	free(str);
	return EXIT_SUCCESS;
}

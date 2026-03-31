#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

#define COLOR_RED "\033[0;91;1m"
#define COLOR_RESET "\033[0m"
void FatalError(const char *message) {
	fprintf(stderr, "%sFatal Error: %s%s\n", COLOR_RED, message, COLOR_RESET);
	exit(1);
}

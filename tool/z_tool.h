#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RAND_STRING_LENGTH 5

int z_set_reuseaddr(int sd);

int z_set_nonblocking(int sd);

int z_atoi(uint8_t *line, size_t n);

char *rand_string();
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define LOG_MAX_LEN 256

typedef struct {
    char *filename;
} loger;

int z_log_init(char *filename);
void z_log(const char *fmt, ...);



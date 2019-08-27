#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*command_handler)(char *buf);

void set_command(char *buf);

struct z_command {
    char *command;
    command_handler handler;
};



int parse_command(char *message);


static struct z_command commands[] = {
    {"set", set_command}
};
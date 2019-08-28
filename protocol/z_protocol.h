#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef H_CONNECTION
#define H_CONNECTION
#include "../connection/z_connection.h"
#endif


#define KEY_TYPE 1
#define VALUE_TYPE 2


typedef int (*command_handler)(zbox *box, char *buf);

int set_command(zbox *box, char *buf);

struct z_command {
    char *command;
    command_handler handler;
};



int parse_command(zbox *box);


static struct z_command commands[] = {
    {"set", set_command}
};
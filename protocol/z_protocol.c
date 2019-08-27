#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "z_protocol.h"
#include "../server/z_server.h"

void set_command(char *buf)
{

}


/**
 * 最简单协议就是以CRLF隔开
 * 目前先支持这2个协议
 * set
 * name
 * 2
 * get
 * name
 **/
int parse_command(char *message)
{
    char *str = message;
    int len = 0;
    while (*str) {
        if (*str == 13) {
            str++;
            if (*str != 13) {

            } else {
                goto done;
            }
            
        } else {
            len++;
        }
        str++;
    }
    if (len == 0) {
        goto error;
    }
    struct z_command *cmd;
    int i;
    printf("commands = %d\n", sizeof(commands) / sizeof(struct z_command));
    for (cmd = commands, i =0; i < sizeof(commands) / sizeof(struct z_command); cmd++,i++) {
        printf("commands = %s\n", cmd->command);
    }

    
done:
    return Z_OK;
error:
    return Z_ERROR;
}
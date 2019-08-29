#include <stdio.h>
#include <stdlib.h>

#ifndef H_CONF
#define H_CONF
#include "../conf/z_conf.h"
#endif



typedef struct {
    int port;
    char *server_ip;
    int aaa;
    char b;
    z_conf *zf;
} z_server;


int server_init(z_server *zs);
void server_start(int listenfd);
static int parse_command_param(z_server *zs, int argc, char **argv);





    





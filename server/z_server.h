#include <stdio.h>
#include <stdlib.h>

#ifndef H_CONF
#define H_CONF
#include "../conf/z_conf.h"
#endif

#define Z_OK 0
#define Z_ERROR 1

typedef struct {
    int port;
    char *server_ip;
    int aaa;
    char b;
    zconf *zf;
} zserver;



    





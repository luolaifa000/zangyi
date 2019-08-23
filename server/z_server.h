#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>


#include "../conf/z_conf.h"
#include "../log/z_log.h"


#define Z_OK 0
#define Z_ERROR 1

typedef struct {
    int port;
    char *server_ip;
    int aaa;
    char b;
    zconf *zf;
} zserver;



    





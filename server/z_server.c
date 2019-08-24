#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

#ifndef H_SERVER
#define H_SERVER
#include "z_server.h"
#endif


#ifndef H_LOG
#define H_LOG
#include "../log/z_log.h"
#endif

#ifndef H_EPOLL
#define H_EPOLL
#include "../epoll/z_epoll.h"
#endif

#ifndef H_CONNECTION
#define H_CONNECTION
#include "../connection/z_connection.h"
#endif

#ifndef H_ALLOC
#define H_ALLOC
#include "../alloc/z_alloc.h"
#endif


static int parse_command_param(zserver *zs, int argc, char **argv)
{
    int opt;
    char *string = "s:p:";

    while ((opt = getopt(argc, argv, string))!= -1) {  
        switch (opt)
        {
            case 's':
                zs->server_ip = optarg;
                break;
            case 'p':
                zs->port = z_atoi(optarg, strlen(optarg));
                break;
            default:
                return Z_ERROR;
                break;
        }
       
    }
    if (zs->port == 0 || zs->server_ip == NULL) {
        return Z_ERROR;
    }
    return Z_OK;
}


int main(int argc, char *argv[]) 
{
    
    int return_status = 0;
    zserver *zs;
    zconf *zf;
    zs = z_alloc(sizeof(*zs));
    zf = init_config(zf);
    zs->zf = zf;

    return_status = z_log_init(zs->zf->logfile);
    
    if (return_status != Z_OK) {
        z_log("z_log_init failure\n");
        exit(Z_ERROR);
    }

    return_status = parse_command_param(zs, argc, argv);

    if (return_status != Z_OK) {
        z_log("invalid option1\n");
        exit(Z_ERROR);
    }

    int sockedfd;
    sockedfd = server_init(zs);
    if (sockedfd < 0) {
        z_log("server_init error\n");
        return Z_ERROR;
    }

    server_start(sockedfd);
    

    /*printf("zs->port = %d\n", zs->port);
    printf("zs->server_ip = %s\n", zs->server_ip);
    printf("zs->zf->logfile = %s\n", zs->zf->logfile);*/

    return Z_OK;
}

int server_init(zserver *zs)
{
    int sockedfd, status; 
    struct sockaddr_in server_addr;  
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(zs->server_ip);
    server_addr.sin_port = htons(zs->port);

    sockedfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockedfd < 0) {
        z_log("create socket error %s\n", strerror(errno));
        return Z_ERROR;
    } else {
        z_log("create socket %d\n", sockedfd);
    }
    status = bind(sockedfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if (status < 0) {
        z_log("bind socket error %s\n", strerror(errno));
        return Z_ERROR;
    } else {
        z_log("bind socket ok!\n");
    }

    status = listen(sockedfd, 5);
    if (status < 0) {
        z_log("listen socket error %s\n", strerror(errno));
        return Z_ERROR;
    } else {
        z_log("listen socket ok!\n");
    }
    return sockedfd;
}


void server_start(int listenfd)
{
    zbox *box;
    box = (zbox *) z_alloc(sizeof(*box));
    box->fd = listenfd;
    epoll_init(listenfd);
    epoll_add_in(listenfd, box);
    epoll_loop();
}











    





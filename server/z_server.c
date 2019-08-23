#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#ifndef H_SERVER
#define H_SERVER
#include "z_server.h"
#endif


#ifndef H_LOG
#define H_LOG
#include "../log/z_log.h"
#endif



int main(int argc, char *argv[]) 
{
    
    int return_status = 0;
    zserver *zs;
    zconf *zf;
    zs = z_alloc(sizeof(*zs));
    zf = init_config(zf);
    
    zs->zf = zf;
    return_status = init_server_command(zs, argc, argv);

    if (return_status != Z_OK) {
        z_log("invalid option\n");
        exit(Z_ERROR);
    }

    return_status = z_log_init(zs->zf->logfile);
    
    if (return_status != Z_OK) {
        z_log("invalid option\n");
        exit(Z_ERROR);
    }

    if (zs->port == 0 || zs->server_ip == NULL) {
        z_log("invalid option\n");
        exit(Z_ERROR);
    }
    

    printf("zs->port = %d\n", zs->port);
    printf("zs->server_ip = %s\n", zs->server_ip);
    printf("zs->zf->logfile = %s\n", zs->zf->logfile);

    return Z_OK;
}


int init_server_command(zserver *zs, int argc, char **argv)
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
                z_log("invalid option -- '%c'\n", opt);
                return Z_ERROR;
                break;
        }
       
    }
    return Z_OK;
}









    





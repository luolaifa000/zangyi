#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "z_server.h"


int main(int argc, char *argv[]) 
{
    struct server_instance *si;
    //printf("%d", sizeof(*si));
    si = z_alloc(sizeof(*si));

    int opt;
    char *string = "s:p:";

    while ((opt = getopt(argc, argv, string))!= -1) {  
        switch (opt)
        {
            case 's':
                /* code */
                si->server_ip = optarg;
                break;
            case 'p':
                si->port = z_atoi(optarg, strlen(optarg));
                break;
            default:
                printf("invalid option -- '%c'", opt);
                return 1;
                break;
        }
       
    }  
    printf("si->port = %d\n", si->port);
        printf("si->server_ip = %s\n", si->server_ip);

   // printf("si = %p", si);
    return 0;
}









    





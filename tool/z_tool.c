#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>

#include "z_tool.h"

#include "../alloc/z_alloc.h"




int z_set_reuseaddr(int sd)
{
    int reuse;
    socklen_t len;

    reuse = 1;
    len = sizeof(reuse);

    return setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse, len);
}


int z_set_nonblocking(int sd)
{
    int flags;

    flags = fcntl(sd, F_GETFL, 0);
    if (flags < 0) {
        return flags;
    }

    return fcntl(sd, F_SETFL, flags | O_NONBLOCK);
}


int z_atoi(uint8_t *line, size_t n)
{
    int value;

    if (n == 0) {
        return -1;
    }

    for (value = 0; n--; line++) {
        if (*line < '0' || *line > '9') {
            return -1;
        }

        value = value * 10 + (*line - '0');
    }

    if (value < 0) {
        return -1;
    }

    return value;
}


char *rand_string()
{
    char *str = (char *)z_alloc(RAND_STRING_LENGTH + 1);
    int i;
     
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);

    char src[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    for (i = 0; i < RAND_STRING_LENGTH; i++) {
        *(str+i) = src[rand() % strlen(src)];
    }
    *(str+i+1) = '\0';
    printf("%s\n", str);//输出生成的随机数。
     
    return str;
}

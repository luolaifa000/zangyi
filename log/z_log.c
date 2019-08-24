#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#include "z_log.h"

#include "../server/z_server.h"

#include "../alloc/z_alloc.h"


static loger *logger;

int z_log_init(char *filename)
{
    logger = z_alloc(sizeof(*logger));
    logger->filename = filename;
    return Z_OK;
}

void z_log(const char *fmt, ...)
{
    //printf("logger->filename = %s\n", logger->filename);
    FILE *fp;
    size_t size, len;
    char buf[LOG_MAX_LEN];
    va_list args;
    ssize_t n;
    struct timeval tv;  
    /*struct timeval {
time_t tv_sec; // seconds 
long tv_usec; // microseconds 
};*/
    fp = fopen(logger->filename, "a+");
    len = 0;            
    size = LOG_MAX_LEN; 

    gettimeofday(&tv, NULL);

    //printf("timeval.tv.tv_sec:%d\n", tv.tv_sec);
    //printf("timeval.tv.tv_usec:%d\n", tv.tv_usec);
    //timeval.tv.tv_sec:1565621144   时间戳
    //timeval.tv.tv_usec:652879     微秒
    buf[len++] = '[';
    //printf("%d, %d, %d\n", size - len, len, tmp_time->tm_year);
    len += strftime(buf + len, size - len, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    len--;
    len += snprintf(buf + len, size - len, "%03ld", tv.tv_usec/1000);
    len--;
    len += snprintf(buf + len, size - len, "] %s:%d ", __FILE__, __LINE__);
    len--;
    len += snprintf(buf + len, size - len, "%s", " [info] ");
    va_start(args, fmt);
    len += vsnprintf(buf + len, size - len, fmt, args);
    va_end(args);
    len--;
    buf[len++] = '\n';

    n = write(2, buf, len);
    n = fwrite(buf, len, 1, fp);
    if (n < 0) {
        printf("write log error : %s\n", strerror(errno));
    }
    fclose(fp);
}


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

#include "z_connection.h"
#include "../log/z_log.h"
#include "../alloc/z_alloc.h"
#include "../epoll/z_epoll.h"
#include "../server/z_server.h"

int connection_accept(zbox *box)
{
    int connfd;
    connfd = accept(box->fd, NULL, NULL);
    if (connfd < 0) {
        z_log("accept socket error %s\n", strerror(errno));
        return Z_ERROR;
    } 

    zbox *cbox;
    cbox = (zbox *)z_alloc(sizeof(*cbox));
    cbox->fd = connfd;
    epoll_add_in(cbox->fd, cbox);
    printf("accept\n");

}

int connection_read(zbox *box)
{
    int n;
    n = read(box->fd, box->rbuf, 255);
    
    if (n < 0) {
        if (errno == EINTR) {
            z_log("read on sd %d not ready - eintr", box->fd);
            return Z_ERROR;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            z_log("recv on sd %d not ready - eagain", box->fd);
            return Z_ERROR;
        } else {
            z_log("recv on sd %d failed: %s", box->fd, strerror(errno));
            return Z_ERROR;
        }
    }
    printf("read data = %s\n", box->rbuf);
}

int connection_write(zbox *box)
{
    printf("write\n");
}
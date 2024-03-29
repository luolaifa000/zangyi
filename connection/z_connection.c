#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

#ifndef H_CONNECTION
#define H_CONNECTION
#include "z_connection.h"
#endif

#include "../log/z_log.h"
#include "../alloc/z_alloc.h"
#include "../epoll/z_epoll.h"
#include "../server/z_server.h"
#include "../tool/z_tool.h"
#include "../protocol/z_protocol.h"



z_buf *get_zbuf()
{
    z_buf *buf;
    buf = (z_buf *) z_alloc(sizeof(*buf));
    buf->size = Z_BUF_SIZE;
    buf->data = (char *) z_alloc((size_t) buf->size);
    return buf;
}

zbox *get_zbox()
{
    zbox *box;
    box = (zbox *)z_alloc(sizeof(*box));
    box->rbuf = get_zbuf();
    box->wbuf = get_zbuf();
    return box;
}

int connection_accept(zbox *box)
{
    int connfd;
    connfd = accept(box->fd, NULL, NULL);
    
    z_set_reuseaddr(connfd);

    if (connfd < 0) {
        z_log("accept socket error %s\n", strerror(errno));
        return Z_ERROR;
    } 

    z_set_nonblocking(connfd);
    

    zbox *cbox;
    cbox = get_zbox();
    cbox->fd = connfd;
    epoll_add_in(cbox->fd, cbox);
    //printf("accept\n");

}

int connection_read(zbox *box)
{
    ssize_t n;
    //安全二进制，必须先重置
    memset(box->rbuf->data, 0, box->rbuf->size);
    n = read(box->fd, box->rbuf->data, box->rbuf->size);
    
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
    } else if (n == 0) {
        return Z_OK;
    }
    *(box->rbuf->data + n) = '\0';
    printf("read data = %s\n", box->rbuf->data);
    parse_command(box);
    epoll_mod_in_out(box->fd, box);
    return Z_OK; 
    
}

int connection_write(zbox *box)
{
    
    printf("write\n");

    char *sss = "nil\r\n";
    ssize_t n;
    int len = strlen(box->wbuf->data);
    if (len > 0) {
        char *s = box->wbuf->data;
        *(s+len) = '\r';
        *(s+len+1) = '\n';
        *(s+len+2) = '\0';
        n = write(box->fd, box->wbuf->data, strlen(box->wbuf->data));
    } else {
        n = write(box->fd, sss, strlen(sss));
    }
    return n;
}
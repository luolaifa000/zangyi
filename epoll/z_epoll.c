#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "z_epoll.h"
#include "../alloc/z_alloc.h"
#include "../log/z_log.h"
#include "../server/z_server.h"
#include "../connection/z_connection.h"


static event_base *eb;

int epoll_init(int listenfd)
{
    eb = z_alloc(sizeof(*eb));
    eb->epoll_size = DEFAULT_EPOLL_SIZE;
    eb->timeout = DEFAULT_EPOLL_TIMEOUT;
    eb->epfd = epoll_create(DEFAULT_EPOLL_SIZE);
    eb->listenfd = listenfd;
    if (eb->epfd < 0) {
        z_log("epoll_create error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_create ok! %d\n", eb->epfd);
    return Z_OK;
}

void epoll_loop()
{
    int nfds, i;
    struct epoll_event *event;
    event = (struct epoll_event *)z_alloc(eb->epoll_size * sizeof(*event));
    for ( ; ; ) {
        nfds = epoll_wait(eb->epfd, event, eb->epoll_size, eb->timeout);
        if (nfds < 0) {
            if (errno == EINTR) {
                continue;
            }
            z_log("epoll wait on e %d with failed:%s", eb->epfd, strerror(errno));
            break;
        }
        printf("nfds = %d\n", nfds);
        //printf("nfds = %d\n", nfds);
        for (i = 0; i < nfds; ++i) {
            zbox *box = (zbox *) event[i].data.ptr;
            if (event[i].events & EPOLLIN) {
                if (box->fd == eb->listenfd) {
                    connection_accept(box);
                } else {
                    connection_read(box);
                }
                //printf("EPOLLIN event[i].events = %d, box->fd = %d, eb->listenfd = %d\n", event[i].events, box->fd, eb->listenfd);
                
            } else if (event[i].events & EPOLLOUT) {
                connection_write(box);
            } else {

            }
        }
    }
}

int epoll_add_in_out(int fd, void *ptr)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = ptr;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_add_in_out ok! %d\n", fd);
    return Z_OK;
}


int epoll_mod_in_out(int fd, void *ptr)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = ptr;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_MOD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_mod_in_out ok! %d\n", fd);
    return Z_OK;
}

int epoll_del_in_out(int fd)
{
    int status;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_DEL, fd, NULL);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    } 
    z_log("epoll_del_in_out ok! %d\n", fd);
    return Z_OK;
}


int epoll_add_in(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = data;
    ev.events = EPOLLIN | EPOLLET;

    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_add_in ok! %d\n", fd);
    return Z_OK;
}

int epoll_mod_in(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = data;
    ev.events = EPOLLIN | EPOLLET;

    status = epoll_ctl(eb->epfd, EPOLL_CTL_MOD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_add_in ok! %d\n", fd);
    return Z_OK;
}


int epoll_del_in(int fd)
{
    return 0;
}

int epoll_add_out(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = data;
    ev.events = EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_add_out ok! %d\n", fd);
    return Z_OK;
}

int epoll_mod_out(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = data;
    ev.events = EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_MOD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    }
    z_log("epoll_add_out ok! %d\n", fd);
    return Z_OK;
}

int epoll_del_out(int fd)
{
    int status;
    struct epoll_event ev; 
    ev.events = (uint32_t)(EPOLLIN | EPOLLET);
    ev.data.ptr = &fd;

    status = epoll_ctl(eb->epfd, EPOLL_CTL_MOD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error! %s\n", strerror(errno));
        return Z_ERROR;
    } 
    z_log("event_del_out ok! %d\n", fd);
    return status;
}





    





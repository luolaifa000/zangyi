#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "z_epoll.h"
#include "../alloc/z_alloc.h"
#include "../log/z_log.h"
#include "../server/z_server.h"


static event_base *eb;

int epoll_init(int sd)
{
    eb = z_alloc(sizeof(*eb));
    eb->epoll_size = DEFAULT_EPOLL_SIZE;
    eb->timeout = DEFAULT_EPOLL_TIMEOUT;
    eb->epfd = epoll_create(DEFAULT_EPOLL_SIZE);
    eb->listenfd = sd;
    if (eb->epfd < 0) {
        z_log("epoll_create error!\n");
        return Z_ERROR;
    }
    z_log("epoll_create ok! %d\n", eb->epfd);
    return Z_OK;
}

void epoll_loop()
{
    int nfds, i;
    struct epoll_event *event;
    event = z_alloc(eb->epoll_size * sizeof(*event));
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
        for (i = 0; i < nfds; ++i) {
            
            if (event[i].events & EPOLLIN) {
                printf("EPOLLIN event[i].events = %d, event[i].data.ptr = %d\n", event[i].events, ((event_base *)event[i].data.ptr)->listenfd);
            } else if (event[i].events & EPOLLOUT) {
                 printf("EPOLLOUT\n");
            } else {

            }
        }
    }
}

int epoll_add_in_out(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = &fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error!\n");
        return Z_ERROR;
    }
    z_log("epoll_add_in_out ok! %d\n", fd);
    return Z_OK;
}


int epoll_add_in(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = eb;
    ev.events = EPOLLIN | EPOLLET;

    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error!\n");
        return Z_ERROR;
    }
    z_log("epoll_add_in ok! %d\n", fd);
    return Z_OK;
}

int epoll_add_out(int fd, void *data)
{
    int status;
    struct epoll_event ev; 
    ev.data.ptr = &fd;
    ev.events = EPOLLOUT | EPOLLET;
    status = epoll_ctl(eb->epfd, EPOLL_CTL_ADD, fd, &ev);
    if (status < 0) {
        z_log("epoll_ctl error!\n");
        return Z_ERROR;
    }
    z_log("epoll_add_out ok! %d\n", fd);
    return Z_OK;
}





    





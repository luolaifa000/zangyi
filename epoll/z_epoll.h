#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>


#define DEFAULT_EPOLL_SIZE 100
#define DEFAULT_EPOLL_TIMEOUT -1
typedef struct {
    int epfd;     
    int listenfd; 
    int timeout;
    int epoll_size;
} event_base;

    
int epoll_init(int sd);
void epoll_loop();
int epoll_add_in_out(int fd, void *data);
int epoll_mod_in_out(int fd, void *ptr);
int epoll_del_in_out(int fd);
int epoll_add_in(int fd, void *data);
int event_del_in(int fd);
int epoll_add_out(int fd, void *data);
int event_del_out(int fd);




#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>



struct event_base {
    int ep;      
    struct epoll_event *event;  
    int nevent;  
};



    





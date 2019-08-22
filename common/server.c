#include "common.h"
#include "server.h"
#include "../hashTable/hashTable.h"
#include "pool.h"


extern hashTable *hash;
extern int serverPort;
extern int channel[2];

//指令回调
command_s command_arr[] = {
    {"run", 3, runCommandHandler,NULL},
    {"walk", 4, walkCommandHandler,NULL},
    {"set", 3, setCommandHandler,NULL},
    {"get", 3, getCommandHandler,NULL},
    {"", 0, NULL}
};


/**
 * 服务器启动初始化
 * 
*/
int serverInit()
{
    struct sockaddr_in servaddr;
    messageBox boxData;
    bzero(&servaddr, sizeof(servaddr));
    bzero(&boxData, sizeof(boxData));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons (serverPort);
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        printf("bind:%s\n",strerror(errno));
        return 1;
    }
    listen(listenfd, MAX_LISTEN_NUM);
    return listenfd;
}


int epollEventInit()
{
    int fd;
    fd = epoll_create(MAX_EPOLL_NUM); 
    return fd;
}

/**
 * epoll事件阻塞
 * 
*/
void epollEventLoop(int listenfd, int epollFd)
{
    struct epoll_event events[5]; 
    int i,n,ret;
    int socket;
    for(;;)
    {
        nfds = epoll_wait(epollFd, events, 5, -1);
        for (i=0; i<nfds; i++)
        {
            messageBox *box = (messageBox *)(events[i].data.ptr);
            socket = box->socket;
            if (socket == listenfd && (events[i].events & EPOLLIN)) {
                handerAccept(listenfd);
            } if (socket == channel[1] && (events[i].events & EPOLLIN)) {
                char *rec = (char *) malloc(100);
                memset(rec,0,100);
                read(socket,rec, 100);
                //printf("rec:%s\n", rec);
                mcLog("heart_request.log",rec);
                memset(rec,0,100);
                sprintf(rec, "pid:%d,i am child!",getpid());
                write(socket, rec, strlen(rec));
                free(rec);
            } else if (socket == channel[0] && (events[i].events & EPOLLIN)) {
                char *rec1 = (char *) malloc(100);
                memset(rec1,0,100);
                read(socket,rec1, 100);
                mcLog("heart_response.log",rec1);
                free(rec1);
            } else if(events[i].events & EPOLLOUT) {
                (*((epollHandle) box->writeHandle))(box);
            } else if(events[i].events & EPOLLIN) {
                (*((epollHandle) box->readHandle))(box);
            } 
        }
    }
}


char* filterHuanHang(char *value)
{
    if(value[strlen(value)-1] == '\n') {
        value[strlen(value)-1] = '\0';
    }
    return value;
}


/**
 * run命令回调
 * 
*/
char* setCommandHandler(char *name, char *value)
{
    hashNode *node = createNode(name, value);
    int flag = hashTableInsert(hash, node);
    printfHashTable(hash);

    if (flag == 0) {
        return "OK";
    } else {
        return NULL;
    }
}

/**
 * walk命令回调
 * 
*/
char* getCommandHandler(char *name, char *value)
{

    char *val = hashTableFind(hash, name);

    if (val) {
        return val;
    } else {
        return NULL;
    }
}


/**
 * run命令回调
 * 
*/
char* runCommandHandler(char *name, char *value)
{
    printf("%s\n", name);
}

/**
 * walk命令回调
 * 
*/
char* walkCommandHandler(char *name, char *value)
{
    printf("%s\n", name);
}

/**
 * 指定回调处理
 * 
*/
char* handlerCommand(char *name)
{
    //printf("name:%s\n",name);
    mcLog("demo.log",name);
    command_s *p = command_arr;
    char *logCommandString = name;

    int i = 0;
    char **argv = defMalloc(sizeof(char *) * 3);
    for (i = 0; i<3;i++) {
        argv[i] = defMalloc(30);
    }
    i = 0;
    char *last;
    int start = 0;
    while (*name) {
        if (start == 0) {
            last = argv[i];
        }
        switch (*name)
        {
            case ' ':
                start = 0;
                i++;
                if (i > 2) {
                    return "format error!";
                }
                break;
            default:
                start = 1;
                *last++ = *name;
                break;
        }
        name++;
    }

    commandLog("command.log", logCommandString);
    /*for (i = 0; i<3;i++) {
        printf("i:%d,argv:%s\n",i,argv[i]);
    }    
    printf("argv0:%s\n",argv[0]);*/
    for ( ; p->len ; p++) {
        if (strcmp(filterHuanHang(argv[0]), p->name) == 0) {
            //printf("argv[1]:%s,argv[2]:%s\n", argv[1], argv[2]);
            char *val = (*(commandHandlerPointer)(p->callback))(filterHuanHang(argv[1]), filterHuanHang(argv[2]));
            return val;
        }
        
    }
    printf("undefined command!\n");
    return NULL;
}
/**
 * 读事件回调
 * 
*/
void readCallback(messageBox *box)
{
    int fd = box->socket;
    int nread;
    
    char *string = (char *)defMalloc(100);
    
    nread = read(fd,string, 100);
    if (nread == -1) {
        perror("read error\n");
    }
    if (nread == 0) {
        perror("close\n");
        close(fd);
    }

    printf("readdata:%s\n", string);
    
    char *response = handlerCommand(string);

    if (!response) {
        response = "null";
    }

    printf("response:%p\n", response);
    printfHashTable(hash);
    epollEventDdl(epfd, EPOLL_CTL_MOD, fd, EPOLLOUT|EPOLLET, readCallback, writeCallback, response);

    defFree(string);
}

/**
 * 注册修改删除事件
 **/
void epollEventDdl(int epollFd, int operation, int fd, int type, epollHandle rFunc,epollHandle wFunc, char *data)
{
    struct epoll_event ev;
    messageBox *boxdata = defMalloc(sizeof(messageBox));
    ev.events = type;
    boxdata->socket = fd;
    boxdata->data = data;
    boxdata->readHandle = rFunc;
    boxdata->writeHandle = wFunc;
    ev.data.ptr = boxdata;
    epoll_ctl(epollFd, operation, fd, &ev);
}

/**
 * 写事件回调
 **/
void writeCallback(messageBox *box)
{
    int fd = box->socket;
    int nwrite;
    nwrite = write(fd,box->data,strlen(box->data));
    if (nwrite == -1) {
        perror("write error\n");
    }
    if (nwrite > 0) {
        epollEventDdl(epfd, EPOLL_CTL_MOD, fd, EPOLLIN|EPOLLET, readCallback, writeCallback, NULL);
    }
}


/**
 * 客户端连接事件回调
 **/
void handerAccept(int listenfd)
{
    int clientFd;
    struct sockaddr_in clientAddr;
    
    socklen_t clientAddrLen;
    clientFd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        perror("error");
    }
    
    printf("accept a new client: %s:%d\n",inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port);
    
    epollEventDdl(epfd, EPOLL_CTL_ADD, clientFd, EPOLLIN|EPOLLET, readCallback, writeCallback, NULL);
}








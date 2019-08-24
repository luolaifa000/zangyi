
typedef struct {
    int fd;
    char rbuf[255];
    char wbuf[255];
} zbox;

int connection_accept(zbox *box);

int connection_read(zbox *box);

int connection_write(zbox *box);
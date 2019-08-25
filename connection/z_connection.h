#define Z_BUF_SIZE 1024


typedef struct {
    int size;
    char *data;
} zbuf;

typedef struct {
    int fd;
    zbuf *rbuf;
    zbuf *wbuf;
} zbox;

zbuf *get_zbuf();

zbox *get_zbox();

int connection_accept(zbox *box);

int connection_read(zbox *box);

int connection_write(zbox *box);
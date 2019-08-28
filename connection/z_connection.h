#define Z_BUF_SIZE 1024


typedef struct {
    int size;
    char *data;
} z_buf;

typedef struct {
    int fd;
    z_buf *rbuf;
    z_buf *wbuf;
} zbox;

z_buf *get_zbuf();

zbox *get_zbox();

int connection_accept(zbox *box);

int connection_read(zbox *box);

int connection_write(zbox *box);
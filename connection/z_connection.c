#include <stdio.h>
#include <stdlib.h>

#include "z_connection.h"

void connection_accept(zbox *box)
{
    printf("accept");
}

void connection_read(zbox *box)
{
    printf("read");
}

void connection_write(zbox *box)
{
    printf("write");
}
#include <stdio.h>
#include <stdlib.h>

void *z_alloc(size_t size)
{
    void *p;
    p = malloc(size);
    if (p == NULL) {
        printf("malloc(%zu) failed", size);
    } else {
        printf("malloc(%zu) at %p", size, p);
    }

    return p;
}


void *z_free(void *p)
{
    free(p);
}

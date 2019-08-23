#include <stdio.h>
#include "z_conf.h"
#include "../alloc/z_alloc.h"

zconf *init_config()
{
    zconf *zc;
    zc = z_alloc(sizeof(*zc));
    if (zc == NULL) {
        z_log("z_alloc error\n");
    }
    zc->logfile = DEFAULT_LOG_FILE;
    return zc;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "z_protocol.h"
#include "../log/z_log.h"
#include "../alloc/z_alloc.h"

#include "../hashtable/z_hashtable.h"

extern z_hashtable *zhash;

int get_command(zbox *box, char *buf)
{
    char *key, *key_start;
    key = key_start = (char *) z_alloc(strlen(buf) + 1);
    char *str = buf;
    int state = KEY_TYPE;
    while (*str) {
        switch (state) {
            case KEY_TYPE:
                if (*str == Z_SPACE) {
                    return Z_ERROR;
                } else if(*str == Z_CR) {
                    str++;
                    if (*str == Z_LF) {
                        goto done;
                    }
                } else {
                    *key = *str;
                    key++;
                    str++;
                }
                break;
            default:
                return Z_ERROR;
                break;
        }
    }
done:
    *key = '\0';

    printf("get_command = %s\n", buf);
    printf("key = %s\n", key_start);
    z_hashnode *value  = hashtable_find(zhash, key_start);
    char *start, *data;
    int size = 0, i =0;
    if (value) {
        start = (char *) value->value;
        data = box->wbuf->data;
        while (*start) {
            *data++ = *start++;
        }
        *data = '\0';
    } else {
        z_memset(box->wbuf->data, box->wbuf->size);
    }
    
    
    printf("write data = %s\n", box->wbuf->data);
    z_memset(box->rbuf->data, box->rbuf->size);
    return Z_OK;
    
}


int set_command(zbox *box, char *buf)
{
    char *key, *key_start, *value, *value_start;
    key = key_start = (char *) z_alloc(255);
    value = value_start= (char *) z_alloc(255);
    char *str = buf;
    int state = KEY_TYPE;
    while (*str) {
        switch (state) {
            case KEY_TYPE:
                if (*str == Z_SPACE) {
                    state = VALUE_TYPE;
                    str++;
                }  else {
                    *key++ = *str++;
                }
                break;
            case VALUE_TYPE:
                if (*str == Z_SPACE) {
                    return Z_ERROR;
                } else if(*str == Z_CR) {
                    str++;
                    if (*str == Z_LF) {
                        goto done;
                    }
                } else {
                    *value = *str;
                    value++;
                    str++;
                }
                break;
            default:
                return Z_ERROR;
                break;
        }
    }

done:
    *key = '\0';
    *value = '\0';

    printf("set_command = %s\n", buf);
    printf("key = %s, value = %s\n", key_start, value_start);
    hashtable_insert(zhash, key_start, value_start);
    //printf_hashtable(zhash);
    z_memset(box->wbuf->data, box->wbuf->size);
    return Z_OK;
}


/**
 * 最简单协议就是以空格隔开
 * 目前先支持这2个协议
 * set
 * name
 * 2
 * get
 * name
 **/
int parse_command(zbox *box)
{
    char *str = box->rbuf->data;
    int len = 0, i = 0, status = Z_OK;
    struct z_command *cmd;
    while (*str) {
        //printf("asnic = %d\n", *str);
        if (*str == Z_SPACE) {
            str++;
            goto done;
            /*
            if (*str != 13) {

            } else {
                goto done;
            }*/
        }
        str++;
    }
done:
    len = str - box->rbuf->data - 1;
    if (len == 0) {
        goto error;
    }
    //printf("commands = %d\n", sizeof(commands) / sizeof(struct z_command));
    for (cmd = commands, i =0; i < sizeof(commands) / sizeof(struct z_command); cmd++,i++) {
        if (strncmp(cmd->command, box->rbuf->data, len) == 0) {
            printf("parse %s command!\n", cmd->command);
            status = cmd->handler(box, str);
            if (status != 0) {
                z_log("parse %s command format error!\n", cmd->command);
                printf("parse %s command format error!\n", cmd->command);
                return Z_ERROR;
            }
            return Z_OK;
        }
        //printf("commands = %s, len = %d\n", cmd->command, len);
    }

    z_log("unknow command : %s\n", box->rbuf->data);
    printf("unknow command : %s\n", box->rbuf->data);
    return Z_ERROR;
error:
    return Z_ERROR;
}
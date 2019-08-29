#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef H_HASHTABLE
#define H_HASHTABLE 1
#include "z_hashtable.h"
#endif

#include "../alloc/z_alloc.h"

#include "../log/z_log.h"


z_hashtable *hashtable_init(int size)
{
    z_hashtable *hash;
    hash = (z_hashtable *) z_alloc(sizeof(z_hashtable));
    hash->size = size;
    hash->countNode = 0;
    hash->func = hash_function;
    hash->bucket = (z_hashnode **) z_alloc(sizeof(z_hashnode) * hash->size);
    return hash;
}

void hashtable_free(z_hashtable *hashtable)
{
    z_free(hashtable->bucket);
    z_free(hashtable);
}

void printf_hashtable(z_hashtable *hash)
{
    int k = 0;
    z_hashnode *p;
    printf("begin print z_hashtable:%d\n", hash->countNode);
    while (k < hash->size) {
        p = ((z_hashnode *) (hash->bucket)) + k;
        while (p) {
            printf("address = %d, size = %d, id = %d, index = %d, key = %s, value = %s\n", p, sizeof(*p), k, p->index, p->key, p->value);
            p = (z_hashnode *) p->next;
        }
        k++;
    }
    
}


z_hashnode *hashtable_insert(z_hashtable *hash, char *key, void *value)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    z_hashnode *p = ((z_hashnode *) (hash->bucket)) + index;
    z_hashnode *pre;
    if (p->key) {
        pre = p;
        while (p) {
            if (strcmp(key, p->key) == 0) {
                //printf("sdfsdfdsf\n");
                p->value = value;
                return Z_OK;
            }
            pre = p;
            p = p->next;
        }
        pre->next = hashnode_create(hash, key, value);
        printf("hashnode_create\n");
    } else {
        printf("hashnode_init\n");
        hashnode_init(hash, p, key, value);
    }
    hash->countNode++;
    return Z_OK;
}

z_hashnode *hashtable_find(z_hashtable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    z_hashnode *p = ((z_hashnode *) (hash->bucket)) + index;
    if (p->key) {
        while(p) {
            if (strcmp(key, p->key) == 0) {
                printf("find key = %s, value = %s\n", p->key, p->value);
                return p;
            }
            p = p->next;
        }
    } 
    printf("not find key = %s\n", p->key);
    return NULL;
}


z_hashnode *hashtable_delete(z_hashtable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    z_hashnode *p = ((z_hashnode *) (hash->bucket)) + index;
    z_hashnode *pre;
    if (p->key && p->next == NULL) {
        printf("delete key = %s, value = %s\n", p->key, p->value);
        hashnode_empty(p);
        hash->countNode--;
        return p;
    } else if (p->key) {
        pre = p;
        while(p) {
            if (strcmp(key, p->key) == 0) {
                printf("delete key = %s, value = %s\n", p->key, p->value);
                if (p->next) {
                    pre->next = p->next;
                } else {
                    pre->next = NULL;
                }
                hash->countNode--;
                return p;
            }
            pre = p;
            p = p->next;
        }
        printf("not delete key = %s\n", p->key);
    } else {
        printf("not delete key = %s\n", p->key);
    }
    return NULL;
}


int hash_function(char *key, int size)
{
    int temp = 0;
    while(*key) {
        temp += (int) *key;
        key++;
    }
    return temp % size;
}



z_hashnode *hashnode_create(z_hashtable *hash, char *key, void *value)
{
    z_hashnode *temp;
    temp = (z_hashnode *) z_alloc(sizeof(z_hashnode));
    temp->key = key;
    temp->value = value;
    temp->index = (*((hashHandler) hash->func))(key, hash->size);
    temp->next = NULL;
    return temp;
}


z_hashnode *hashnode_init(z_hashtable *hash, z_hashnode *hashnode, char *key, void *value)
{
    hashnode->key = key;
    hashnode->value = value;
    hashnode->index = (*((hashHandler) hash->func))(key, hash->size);
    hashnode->next = NULL;
    return hashnode;
}


z_hashnode *hashnode_empty(z_hashnode *hashnode)
{
    hashnode->key = NULL;
    hashnode->value = NULL;
    hashnode->index = 0;
    hashnode->next = NULL;
    return hashnode;
}
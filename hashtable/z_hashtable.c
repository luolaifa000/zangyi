#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef H_HASHTABLE
#define H_HASHTABLE 1
#include "z_hashtable.h"
#endif

#include "../alloc/z_alloc.h"


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
        p = (z_hashnode *) (hash->bucket + k);
        while (p) {
            printf("id = %d, index = %d,key = %s,value = %s\n", k, p->index, p->key, p->value);
            p = (z_hashnode *) p->next;
        }
        k++;
    }
    exit(1);
}


int hashtable_insert(z_hashtable *hash, char *key, void *value)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    z_hashnode *p = (z_hashnode *) (hash->bucket + index);
    //z_hashnode *p_pre;
    if (p->key) {
        while (p->next) {
            p = p->next;
        }
        p->next = hashnode_create(hash, key, value);
    } else {
        printf("init");
        hashnode_init(hash, p, key, value);
    }
    hash->countNode++;
    return ;
}

char *hashtable_find(z_hashtable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    if (hash->bucket[index]) {
        z_hashnode *p = hash->bucket[index];
        while(p) {
            if (strcmp(key, p->key) == 0) {
                return p->value;
            }
            p = p->next;
        }
    } 
    return NULL;
}


int hashtable_delete(z_hashtable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    if (hash->bucket[index]) {
        z_hashnode *p = hash->bucket[index];
        z_hashnode *p_pre = p;
        while(p) {
            if (strcmp(key, p->key) == 0) {
                //尾节点
                if (p->next == NULL) {
                    p_pre->next = NULL;
                } else {
                    p_pre->next = p->next;
                }
                hash->countNode--;
                return 1;
            }
            p_pre = p;
            p = p->next;
        }
    } 
    return 0;
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
    temp = (z_hashnode *) z_alloc(sizeof(z_hashnode *));
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
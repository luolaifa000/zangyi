#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int index;
    char *key;
    void *next;
    void *value;
} z_hashnode;

typedef struct {
    int size;
    void *func;
    z_hashnode **bucket;
    int countNode;
} z_hashtable;



z_hashtable *hashtable_init(int size);

void hashtable_free(z_hashtable *hashtable);

typedef int (*hashHandler)(char *key, int size);

int hash_function(char *key, int size);

int hashtable_insert(z_hashtable *hash, char *key, void *value);

char *hashtable_find(z_hashtable *hash, char *key);

int hashtable_delete(z_hashtable *hash, char *key);

z_hashnode *hashnode_create(z_hashtable *hash, char *key, void *value);

z_hashnode *hashnode_init(z_hashtable *hash, z_hashnode *hashnode, char *key, void *value);

void printf_hashtable(z_hashtable *hash);
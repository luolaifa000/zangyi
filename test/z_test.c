#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "z_test.h"

#include "../hashtable/z_hashtable.h"

#include "../tool/z_tool.h"

void test_hashtable()
{
    z_hashtable *hashtalbe;
    hashtalbe = hashtable_init(50);

    hashtable_insert(hashtalbe, "name", "luolaifa");
    hashtable_insert(hashtalbe, "age", "22");
    hashtable_insert(hashtalbe, "name", "zhangzhen");

    hashtable_insert(hashtalbe, "name", "luoxianjun");

    

    int i = 40;
    char *key, *value;
    for (i = 0; i< 40; i++) {
        key = rand_string();
        hashtable_insert(hashtalbe, key, key);
    }

    hashtable_find(hashtalbe, "age");
    hashtable_find(hashtalbe, "name");
    hashtable_delete(hashtalbe, "name");
    printf_hashtable(hashtalbe);
    hashtable_free(hashtalbe);
    
}


int main(int argc, char *argv[]) 
{
    test_hashtable();
    
    return 0;
}
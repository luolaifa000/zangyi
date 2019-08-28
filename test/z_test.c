#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "z_test.h"

#include "../hashtable/z_hashtable.h"

void test_hashtable()
{
    z_hashtable *hashtalbe;
    hashtalbe = hashtable_init(10);

    hashtable_insert(hashtalbe, "name", "luolaifa");
    printf_hashtable(hashtalbe);

    hashtable_free(hashtalbe);
    
}


int main(int argc, char *argv[]) 
{
    test_hashtable();
    
    return 0;
}
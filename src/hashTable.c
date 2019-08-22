#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
    int index;
    char *key;
    void *next;
    void *pre;
    char *value;
} hashNode;

typedef struct {
    int size;
    void *func;
    hashNode **bucket;
    int countNode;

} hashTable;

hashTable* hashInit(int size);

typedef int (*hashHandler)(char *key, int size);

int hashFunction(char *key, int size);

int hashTableInsert(hashTable *hash, hashNode *node);

char* hashTableFind(hashTable *hash, char *key);

int hashTableDelete(hashTable *hash, char *key);

void *defMalloc(int size);

hashNode* createNode(const char *string, const char *value);

void printfHashTable(hashTable *hash);

int main()
{
    hashTable *hash;
    hash = hashInit(16);
    

    //插入数据
    char arr[][10] = {{"java"},{"php"},{"c"},{"python"},{"go"},{"20"}};
    int i = 0;
    for (i; i<6 ; ++i)
    {
        hashNode *temp = createNode(arr[i], arr[i]);
        hashTableInsert(hash, temp);
    }

    //遍历数据打印一下
    printfHashTable(hash);

    char *find = hashTableFind(hash, "dd");
    printf("find:%s\n", find);


    hashTableDelete(hash, "java");
    printfHashTable(hash);
    free(hash->bucket);
    free(hash);
    return 0;
}


hashTable* hashInit(int size)
{
    hashTable *hash;
    hash = (hashTable *) defMalloc(sizeof(hashTable));
    hash->size = size;
    hash->countNode = 0;
    hash->func = hashFunction;
    hash->bucket = (hashNode **) defMalloc(sizeof(hashNode *) * hash->size);
    return hash;
}

void printfHashTable(hashTable *hash)
{
    int k = 0;
    hashNode *p;
    printf("begin print hashTable:%d\n",hash->countNode);
    while(k++ < hash->size){
        /* code */
        if (hash->bucket[k]) {
            printf("index:%d,key:%s,value:%s\n",hash->bucket[k]->index,hash->bucket[k]->key,hash->bucket[k]->value);
            p = hash->bucket[k];
            while(p->next) {
                printf("index:%d,key:%s,value:%s\n",((hashNode *)(p->next))->index,((hashNode *)(p->next))->key,((hashNode *)(p->next))->value);
                p = (hashNode *) p->next;
            }
        }
            
    }
}


int hashTableInsert(hashTable *hash, hashNode *node)
{
    printf("key:%s\n", node->key);
    int index = (*((hashHandler) hash->func))(node->key, hash->size);
    node->index = index;
    printf("index:%d\n", index);
    hashNode *p = hash->bucket[index];
    hashNode *p_pre;
    if (p) {
        if (strcmp(node->key, p->key) == 0) {
            return 1;
        }
        if (strcmp(node->key, p->key) < 0) {
            hash->bucket[index] = node;
            node->next = p;
            hash->countNode++;
            return 0;
        }
        //地址冲突，找到链表尾节点在后面插入
        p_pre = p;
        p = p->next;
        while(p) {
           
            if (strcmp(node->key, p->key) == 0) {
                return 1;
            }
            if (strcmp(node->key, p->key) < 0) {
                p_pre->next = node;
                node->next = p;
                hash->countNode++;
                return 1;
            }
            p_pre = p;
            p = p->next;
        }
        p_pre->next = node;

    } else {
        hash->bucket[index] = node;
    }
    hash->countNode++;
    return 0;
}

char* hashTableFind(hashTable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    if (hash->bucket[index]) {
        hashNode *p = hash->bucket[index];
        while(p) {
            if (strcmp(key, p->key) == 0) {
                return p->value;
            }
            p = p->next;
        }
    } 
    return NULL;
}


int hashTableDelete(hashTable *hash, char *key)
{
    int index = (*((hashHandler) hash->func))(key, hash->size);
    if (hash->bucket[index]) {
        hashNode *p = hash->bucket[index];
        hashNode *p_pre = p;
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


int hashFunction(char *key, int size)
{
    int temp = 0;
    while(*key) {
        temp += (int) *key;
        key++;
    }
    return temp % size;
}

void *defMalloc(int size)
{
    void *temp = malloc(size);
    memset(temp,0,size);
    return temp;
}


hashNode* createNode(const char *string, const char *value)
{
    hashNode *temp;
    temp = (hashNode *) defMalloc(sizeof(hashNode));

    temp->key = (char *)string;
    temp->value = (char *)value;
    temp->index = 0;
    temp->next = NULL;
    temp->pre = NULL;
    return temp;
}
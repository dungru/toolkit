#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUCKET_SIZE 10
struct hash_entry {
    int key;
    struct hash_entry *next;
};

typedef struct {
    struct hash_entry **bucket;
} MyHashSet;

int myHash_func(int key) {
    return key % BUCKET_SIZE;
}
bool myHashSetContains(MyHashSet* obj, int key);
/** Initialize your data structure here. */
MyHashSet* myHashSetCreate() {
    MyHashSet* object = malloc(sizeof(MyHashSet));
    object->bucket = malloc(sizeof(struct hash_entry*) * BUCKET_SIZE);
    memset(object->bucket, 0 , sizeof(struct hash_entry*) * BUCKET_SIZE);
    return object;
}

void myHashSetAdd(MyHashSet* obj, int key) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry **curr;
    if (myHashSetContains(obj, key)) {
        return;
    }
    curr = &obj->bucket[bucket_idx];
    while (*curr != NULL) {
        *curr = (*curr)->next;
    }
    *curr = malloc(sizeof(struct hash_entry));
    (*curr)->key = key;
    (*curr)->next = NULL;
}

void myHashSetRemove(MyHashSet* obj, int key) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry *curr, *prev;
    curr = obj->bucket[bucket_idx];
    prev = obj->bucket[bucket_idx];
    while(curr != NULL) {
        if (curr->key == key) {
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/** Returns true if this set did not already contain the specified element */
bool myHashSetContains(MyHashSet* obj, int key) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry *curr;
    bool find = false;
    curr = obj->bucket[bucket_idx];

    while(curr!= NULL) {
        if(curr->key == key) {
            find = true;
            break;
        }
        curr = curr->next;
    }
    return find;
}

void myHashSetFree(MyHashSet* obj) {
    free(obj);
}

int main(int argc, char const *argv[])
{
    MyHashSet* obj = myHashSetCreate();
    bool param_3;
    myHashSetAdd(obj, 1);
    myHashSetAdd(obj, 2);
    param_3 = myHashSetContains(obj, 1);
    param_3 = myHashSetContains(obj, 3);
    myHashSetAdd(obj, 2);
    param_3 = myHashSetContains(obj, 2);
    myHashSetRemove(obj, 2);
    param_3 = myHashSetContains(obj, 2);
    printf("[dungru:%d:%s] %d\n", __LINE__, __FUNCTION__, param_3);
    
    myHashSetFree(obj);

    return 0;
}


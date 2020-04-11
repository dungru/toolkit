#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUCKET_SIZE 10
struct hash_entry {
    int key;
    int value;
    struct hash_entry *next;
};
typedef struct {
    struct hash_entry *bucket[BUCKET_SIZE];
} MyHashMap;
int myHash_func(int key) {
    return key % BUCKET_SIZE;
}
/** Initialize your data structure here. */
MyHashMap* myHashMapCreate() {
    int idx = 0;
    MyHashMap* object = malloc(sizeof(MyHashMap));
    for (idx = 0; idx < BUCKET_SIZE; idx++) {
        object->bucket[idx] = malloc(sizeof(struct hash_entry));
        memset(object->bucket[idx], 0 , sizeof(struct hash_entry));
    }

    return object;
}

void hash_table_dump(MyHashMap *object) {
    int idx = 0;
    struct hash_entry *curr;
    for (idx = 0; idx < BUCKET_SIZE; idx++) {
        curr = object->bucket[idx];
        printf("[%d]:", idx);
        while (curr != NULL) {
            printf("0x%x, ", curr->key);
            curr = curr->next;
        }
        printf("\n");
    }
}

/** value will always be non-negative. */
void myHashMapPut(MyHashMap* obj, int key, int value) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry *curr;
    curr = obj->bucket[bucket_idx];
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = malloc(sizeof(struct hash_entry));
    curr->key = key;
    curr->value = value;
    curr->next->next = NULL;
}

/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
int myHashMapGet(MyHashMap* obj, int key) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry *curr;
    int find = -1;
    curr = obj->bucket[bucket_idx];

    while(curr->next != NULL) {
        if(curr->key == key) {
            find = curr->value;
            break;
        }
        curr = curr->next;
    }
    return find;
}

/** Removes the mapping of the specified value key if this map contains a mapping for the key */
void myHashMapRemove(MyHashMap* obj, int key) {
    int bucket_idx;
    bucket_idx = myHash_func(key);
    struct hash_entry **indirect, *curr;
    indirect = &obj->bucket[bucket_idx];
    while((*indirect)->key != key) {
        indirect = &(*indirect)->next;
        curr = *indirect;
    }
    *indirect = curr->next;
}

void myHashMapFree(MyHashMap* obj) {
    free(obj);
}

int main(int argc, char const *argv[])
{
    MyHashMap *obj = myHashMapCreate();
    myHashMapPut(obj, 1, 1);          
    myHashMapPut(obj, 2, 2);         

    myHashMapRemove(obj, 2);          // remove the mapping for 2
    myHashMapGet(obj, 2);            // returns -1 (not found)
    hash_table_dump(obj);

    return 0;
}
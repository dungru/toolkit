#include <stdio.h>
#define NUM_OF_LETTER (27)

typedef struct Trie {
    struct Trie *subs[NUM_OF_LETTER];
    char letters[NUM_OF_LETTER];
    unsigned char boolean[NUM_OF_LETTER];
} Trie;
/** Initialize your data structure here. */
Trie* trieCreate() {
    Trie *obj = malloc(sizeof(struct Trie));
    memset(obj->letters, 0xff, sizeof(obj->letters));
    memset(obj->boolean, 0, sizeof(obj->boolean));
    memset(&obj->subs[0], 0, sizeof(obj->subs));
    return obj;
}

/** Inserts a word into the trie. */
void trieInsert(Trie* obj, char* word) {
    int pos;
    if (obj == NULL)
        return;
    while (*word != '\0') {
        pos = *word - 'a' + 1;
        obj->letters[pos] = *word++;
        if(obj->subs[pos] == NULL)
            obj->subs[pos] = trieCreate();
        obj = obj->subs[pos];
    }
    obj->boolean[pos] = 1;
    return;
}

/** Returns if the word is in the trie. */
bool trieSearch(Trie* obj, char* word) {
    int pos;
    while (*word != '\0') {
        pos =  *word - 'a' + 1;
        if(obj->letters[pos] != *word) {
            return false;
        }
        word++;
        obj = obj->subs[pos];
    }
    if(obj->boolean[pos] == 1) {
        return true;
    }
    return false;
}

/** Returns if there is any word in the trie that starts with the given prefix. */
bool trieStartsWith(Trie* obj, char* prefix) {
    int pos;
    if (obj == NULL)
        return false;
    while (*prefix != '\0'){
        pos = *prefix - 'a' + 1;
        if (pos < 0 || obj->letters[pos] != *prefix) {
           return false; 
        }
        if (*++prefix != '\0') {
            if (obj->subs[pos] == NULL) {
                return false;
            }
            obj = obj->subs[pos];
        }
    }
    return 1;
}

void trieFree(Trie* obj) {
    int i;
    for (i = 0; i < NUM_OF_LETTER; i++) {
        if (obj->subs[i] != NULL) {
            trieFree(obj->subs[i]);
        }
    }
    free(obj);
    return;
}

/**
 * Your Trie struct will be instantiated and called as such:
 * struct Trie* obj = trieCreate();
 * trieInsert(obj, word);
 * bool param_2 = trieSearch(obj, word);
 * bool param_3 = trieStartsWith(obj, prefix);
 * trieFree(obj);
 */
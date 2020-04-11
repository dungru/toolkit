char* addBoldTag(char* s, char** dict, int dictSize) {
    //use a array to record matches
    int i, j, k, sz, len, retlen;
    int *chk;
    char *ret;
    if (s == NULL) return NULL;
    if (dict == NULL || dictSize == 0) return s;
    sz = strlen(s);
    chk = calloc(sz, sizeof(int)); //check NULL
    
    for (i = 0; i < dictSize; i++) {
        // compare dict[i] and s
        len = strlen(dict[i]);
        for (j = 0; j <= sz - len; j++) {
            if (strncmp(s+j, dict[i], len) == 0) {
                for (k = 0; k < len; k++) chk[j+k] = 1;
            }
        }
    }
    if (chk[0]) retlen = 4; else retlen = 1;
    for (i = 1; i < sz; i++) {
        retlen ++;
        if (chk[i] == 1 && chk[i-1] == 0) {
            retlen += 3;
        } else if (chk[i] == 0 && chk[i-1] == 1) {
            retlen += 4;
        }
    }
    if (chk[sz-1]) retlen += 3;
    retlen ++; ///zero ending
    ret = malloc(retlen);
    j = 0;
    if (chk[0]) {
        strcpy(ret, "<b>");
        j = 3;
        ret[j++] = s[0];
    } else {
        ret[0] = s[0];
        j = 1;
    }
    for (i = 1; i < sz; i++) {
        if (chk[i] != chk[i-1]) {
            if (chk[i] == 0) {
                strcpy(ret+j, "</b>"); j +=4;
                ret[j++] = s[i];
            } else {
                strcpy(ret+j, "<b>"); j +=3;
                ret[j++] = s[i];                
            }
        } else {
            ret[j++] = s[i];
        }
    }
    if (chk[sz-1]) {
        strcpy(ret+j, "</b>"); j+=4;
    }
    ret[j] = 0;
    return ret;
    
}



// My solution
#include <string>
char * my_strstr(char *str, char *substr) {
    char *s = str;
    char *p = substr;
    if (*p == 0) {
        return str;
    }
    for ( ; *str != 0; str++) {
        if(*str != *p)
            continue;
        s = str;
        while (1) {
            if (*p == 0)
                return str;
            if (*s++ != *p++)
                break;
        }
        p = substr;
    }
    return NULL;
}

char* addBoldTag(char* s, char** dict, int dictSize) {
    char *result = NULL;
    unsigned char map[2048] = {0};
    char *pos_str;
    int pos;
    int i = 0;
    int str_size = strlen(s);
    for (int j = 0; j< str_size; j++) {
        for (i = 0; i < dictSize; i++) {
            if((pos_str = my_strstr(s + j, dict[i]))) {
                memset(map + (pos_str - s), 1, strlen(dict[i]));
            }
        } 
    }
    
    result = malloc (2048);
    memset(result, 0, 2048);
    pos = 0;
    for (i = 0; i < str_size; ++i) {
        if (map[i] && (i == 0 || !map[i - 1])) {
            strcpy(result + pos, "<b>");
            pos += 3;
        }
        result[pos++] = s[i];
        if (map[i] && (i == str_size - 1 || !map[i + 1])) {
            strcpy(result + pos, "</b>");
            pos += 4;
        }
    }
    
    return result;
    
}
#include <stdlib.h>
#include <string.h>
#include "mystring.h"

size_t next_alphanum(int start, const char *str) {
    while (str[start] == ' ') {
        start++;
    }
    return start;
}

size_t next_terminal(int start, const char *str) {
    while(str[start] != ' ' && str[start] != '\0') {
        start++;
    }
    return start;
}

strarray_t *strsplit(const char *str) {
    strarray_t *ret = malloc(sizeof(strarray_t));
    
    //handle base case str == '\0'
    if (str[0] == '\0') {
        ret->data = NULL;
        ret->length = 0;
        return ret;
    }
    
    //first count the number of words
    size_t idx = 1;
    size_t num_words = 0;
    while (str[idx] != '\0') {
        if (str[idx] == ' ' && str[idx-1] != ' ') {
            num_words++;
        }
        idx++;
    }

    if (str[idx-1] != ' ') {
        num_words++;
    }
    
    ret->data = malloc(sizeof(char*) * num_words);
    ret->length = num_words;

    //move to first nonspace char
    idx = 0;
    size_t ret_idx = 0;
    while (num_words) {
        idx = next_alphanum(idx, str);
        size_t end = next_terminal(idx, str);
        char *next = malloc(sizeof(char) * (end-idx+1));

        //copy into next
        for (size_t i=idx; i<end; i++) {
            next[i-idx] = str[i];
        }
        next[end-idx] = '\0';
        ret->data[ret_idx] = next;
        ret_idx++;
        num_words--;
        idx = end;
    }

    return ret;

}

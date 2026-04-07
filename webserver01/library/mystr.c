#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"
#include "mystr.h"

ssize_t mystr_indexof(const char *str, const char sep, size_t start) {
    for (size_t i = start; i < strlen(str); ++i) {
        if (str[i] == sep) {
            return i;
        }
    }
    return -1;
}

strarray_t *mystr_split(const char *str, const char sep) {
    int len = strlen(str);
    int curr_idx = 0;
    int count = 0;

    while (curr_idx < len) {
        while (curr_idx < len && str[curr_idx] == sep) {
            curr_idx++;
        }
        if (curr_idx >= len) {
            break;
        }
        int end = mystr_indexof(str, sep, curr_idx);
        count += 1;
        if (end == -1) {
            break;
        }
        curr_idx = end+1;
    }

    strarray_t *ret = strarray_init(count);
    curr_idx = 0;
    size_t insert_idx = 0;

    while (curr_idx < len) {
        while (curr_idx < len && str[curr_idx] == sep) {
            curr_idx += 1;
        }
        if (curr_idx >= len) {
            break;
        }
        int end = mystr_indexof(str, sep, curr_idx);
        if (end == -1) {
            end = len;
        }
        int word_len = end - curr_idx;
        ret->data[insert_idx] = malloc(word_len + 1);
        memcpy(ret->data[insert_idx], str + curr_idx, word_len);
        ret->data[insert_idx][word_len] = '\0';
        insert_idx += 1;

        if (end == -1) {
            break;
        }
        curr_idx = end + 1;
    }
    return ret;
}

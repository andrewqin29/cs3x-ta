#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *string_reverse(char *s) {
    int s_len = strlen(s);
    char *ret = malloc(sizeof(char)*s_len + 1);

    for (size_t i=0; i<s_len; i++) {
        ret[i] = s[s_len-1-i];
    }
    ret[s_len] = '\0';
    return ret;
}

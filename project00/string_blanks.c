#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **string_blanks(char *s) {
    int s_len = strlen(s);
    char **ret = malloc(sizeof(char*) * s_len);

    for (size_t i=0; i<s_len; i++) {
        ret[i] = malloc(sizeof(char) * s_len + 1);
        strcpy(ret[i], s);
        ret[i][i] = '_';
    }
    return ret;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"

strarray_t *strarray_init(size_t length) {
    strarray_t* ret = malloc(sizeof(strarray_t));
    ret->length = length;
    ret->data = malloc(sizeof(char*)*length);

    //set all strings to NULL
    for (size_t i=0; i<ret->length; ++i) {
        ret->data[i] = NULL;
    }
    
    return ret;
}

void strarray_free(strarray_t *arr) {
    //first free all the strings
    //then the data** container
    //then the strarray_t itself

    for(size_t i=0; i<arr->length; ++i) {
        free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

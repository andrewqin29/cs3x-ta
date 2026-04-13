#include <stdlib.h>
#include <packed_strarray.h>
#include <string.h>
#include <assert.h>

struct packed_strarray {
  const char *data;
  /*
  * Offsets is an array of size_t's where offsets[i] is the number of characters
  * past the beginning `data` where the corresponding i-th string begins.
  */
  size_t *offsets;
  size_t length;
};

packed_strarray_t *packed_strarray_init(char **strings, size_t num) {
    packed_strarray_t *ret = malloc(sizeof(packed_strarray_t));
    ret->offsets = malloc(sizeof(size_t) * num);
    ret->length = num;
    
    //first count the total space we need
    size_t total_len = 0;
    for (size_t i=0; i<num; ++i) {
        //include +1 for null terminator for every string
        size_t curr_len = strlen(strings[i]);
        ret->offsets[i] = total_len;
        total_len += curr_len + 1; 
    }

    char *data = malloc(sizeof(char) * total_len);
    size_t idx = 0;
    for (size_t i=0; i<num; ++i) {
        char *curr_word = strings[i];
        size_t cw_len = (size_t)strlen(curr_word);
        for (size_t j=0; j<cw_len; ++j) {
            data[idx] = curr_word[j];
            idx++;
        }
        data[idx] = '\0';
        idx++;
    }

    ret->data = (const char *) data;
    return ret;
}

void packed_strarray_free(packed_strarray_t *array) {
    free((char *)array->data);
    free(array->offsets);
    free(array);
}

size_t packed_strarray_len(packed_strarray_t *array) {
  return array->length;
}

const char * const packed_strarray_get(packed_strarray_t *array, size_t idx) {
    assert(idx < array->length);
    size_t total_offset = array->offsets[idx];
    return (const char *)&array->data[total_offset];
}

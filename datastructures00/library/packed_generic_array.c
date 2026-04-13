#include <stdlib.h>
#include <packed_generic_array.h>
#include <string.h>
#include <assert.h>

struct packed_generic_array {
  const void *data;
  size_t data_size;
  size_t length;
};

packed_generic_array_t *packed_generic_array_init(void **data, size_t num, size_t data_size) {
    packed_generic_array_t *ret = malloc(sizeof(packed_generic_array_t));
    ret->length = num;
    ret->data_size = data_size;
    
    char* temp = malloc(num * data_size);
    for (size_t i=0; i<num; ++i) {
        for (size_t j=0; j<data_size; ++j) {
            temp[i*data_size + j] = ((char *)data[i])[j];
        }
    }

    ret->data = (const void *)temp;
    return ret;
}


void packed_generic_array_free(packed_generic_array_t *array) {
  free((char*)array->data);
  free(array);
}

size_t packed_generic_array_len(packed_generic_array_t *array) {
    return array->length;
}

const void * const packed_generic_array_get(packed_generic_array_t *array, size_t idx) {
    assert(idx < array->length);
    size_t total_offset = array->data_size * idx;
    return (const void *)&(array->data[total_offset]);
}

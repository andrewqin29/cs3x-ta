#ifndef packed_strarray_H
#define packed_strarray_H

struct packed_strarray;

typedef struct packed_strarray packed_strarray_t;

/*
* Given `strings` and the `num` of strings being pointed to, this function copies
* the data into a contiguous heap-allocated piece of memory with no padding/garbage bytes.
*
* Ownership of `data` is not passed to the packed_strarray_t.
*/
packed_strarray_t *packed_strarray_init(char **strings, size_t num);

/*
* Frees a packed string array and all data owned by it.
*/
void packed_strarray_free(packed_strarray_t *array);

/*
* Returns the length of the given packed string array.
*/
size_t packed_strarray_len(packed_strarray_t *array);

/*
* Returns a pointer to data owned by the packed string array corresponding to the
* index provided.
*
* Take note of the return type. The first `const` indicates that the
* data pointed to should be unchanged, and the second `const` indicates 
* the pointer itself should not be moved. While neither are enforced by
* C, they should be maintained properly.
*/
const char * const packed_strarray_get(packed_strarray_t *array, size_t idx);

#endif

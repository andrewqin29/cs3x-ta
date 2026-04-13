#ifndef packed_generic_array_H
#define packed_generic_array_H

struct packed_generic_array;

typedef struct packed_generic_array packed_generic_array_t;

/*
* Given `data` (with type UNKNOWN to the array), the `num` of elements,
* and the `data_size` of each piece of data, this function copies the data
* into a contiguous heap-allocated piece of memory with no padding/garbage bytes.
*
* Ownership of `data` is not passed to the packed_generic_array_t.
*/
packed_generic_array_t *packed_generic_array_init(void **data, size_t num, size_t data_size);

/*
* Frees a packed generic array and all data owned by it.
*/
void packed_generic_array_free(packed_generic_array_t *array);

/*
* Returns the length of the given packed generic array.
*/
size_t packed_generic_array_len(packed_generic_array_t *array);

/*
* Returns a pointer to data owned by the packed generic array corresponding to the
* index provided.
*
* Take note of the return type. The first `const` indicates that the
* data pointed to should be unchanged, and the second `const` indicates 
* the pointer itself should not be moved. While neither are enforced by
* C, they should be maintained properly.
*/
const void * const packed_generic_array_get(packed_generic_array_t *array, size_t idx);

#endif

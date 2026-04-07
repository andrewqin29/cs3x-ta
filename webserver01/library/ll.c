#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ll.h"


typedef struct node {
    entry_t entry;
    struct node *next;
} node_t;

typedef struct ll_map {
    size_t length;
    node_t *head;
} ll_map_t;

/**
 * Initialize a new node with the given key and value.
 * 
 * The returned node should be heap-allocated with malloc and it takes ownership
 * of the arguments so the caller should not modify or free them afterward.
 */
static node_t *node_init(char *key, char* value);

/**
 * Frees a linked list node and all following nodes.
 * 
 * Also frees the keys and values since they are owned by the node.
 * 
 * Passing NULL to this function is valid (and will do nothing).
 */
static void node_free(node_t *curr);

static node_t *node_init(char *key, char* value) {
    node_t *ret = malloc(sizeof(node_t));
    ret->entry.key = key;
    ret->entry.value = value;
    return ret;
}

static void node_free(node_t *curr) {
    while (curr) {
        free(curr->entry.key);
        free(curr->entry.value);
        node_t *next = curr->next;
        free(curr);
        curr = next;
    }
}

ll_map_t *ll_init(void) {
    ll_map_t *ret = malloc(sizeof(ll_map_t));
    ret->length = 0;
    ret->head = NULL;
    return ret;
}

void ll_free(ll_map_t *dict) {
    node_free(dict->head);
    free(dict);
}

char *ll_put(ll_map_t *dict, char *key, char *value) {
    node_t *curr = dict->head;
    //first see if key exists in the ll
    while(curr) {
        if (strcmp(curr->entry.key, key) == 0) {
            //key already exists so the one passed in must be freed since we have
            //two identical copies
            free(key);

            //the returned entry is owned by the caller
            char *ret = curr->entry.value;

            curr->entry.value = value;
            return ret;
        }
        curr = curr->next;
    }

    //key does not exist already, create new node and put at head of ll
    node_t *old_head = dict->head;
    dict->head = node_init(key, value);
    dict->head->next = old_head;
    dict->length++;
    return NULL;
}

char *ll_get(ll_map_t *dict, char *key) {
    node_t *curr = dict->head;
    while (curr) {
        if (strcmp(curr->entry.key, key) == 0) {
            return curr->entry.value;
        }
        curr = curr->next;
    }
    return NULL;
}

strarray_t *ll_get_keys(ll_map_t *dict) {
    strarray_t *ret = strarray_init(dict->length);
    node_t *curr = dict->head;
    int idx = 0;

    while (curr) {
        int key_len = strlen(curr->entry.key);
        char *curr_key = malloc(sizeof(char) * key_len + 1);
        strcpy(curr_key, curr->entry.key, key_len);
        curr_key[key_len] = '\0';
        ret->data[idx] = curr_key;
        idx++;
        curr = curr->next;
    }
    return ret;

}

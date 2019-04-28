#ifndef spset_E5B1474873A2487f92C1EDB0FA95CAEA
#define spset_E5B1474873A2487f92C1EDB0FA95CAEA

/**
 * Description: Implementation for a sparse set of integers in the range [0, m).
 * Requirement: È(sizeof(int) * m) memory
 *
 * Insertion & deletion are constant time operations.
 * Set allocation is a constant time operation when memory allocation is constant.
 * Iteration through the set is a constant time operation [O(n) where n is the number of
 * items in the set].
 * Clearing the set is a constant time operation.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * The set, by default, is defined for unsigned int; simply define
     * this macro before including this header to change the element type.
     */
#ifndef SPARSE_SET_ELEMENT_TYPE
#define SPARSE_SET_ELEMENT_TYPE unsigned int
#endif

typedef struct {
    SPARSE_SET_ELEMENT_TYPE *dense;
    size_t *sparse;
    size_t cardinality;
    size_t capacity;
} sparse_set_t;

/**
 * Allocates and initializes a new sparse set, with the given capacity.
 */
sparse_set_t sparse_set_init (size_t capacity);

/**
 * Clears the sparse set.
 * This is a constant time operation.
 */
void sparse_set_clear (sparse_set_t *set);

/**
 * Returns the cardinality of the sparse set.
 */
const size_t sparse_set_size (sparse_set_t *set);

/**
 * Returns the maximum capacity of the sparse set.
 * Indices can be in the range [0, capacity).
 */
const size_t sparse_set_capacity (sparse_set_t *set);

/*
 * Returns true if elem is a member of the set, false otherwise.
 */
bool sparse_set_is_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem);

/**
 * Insert the given element in the set, provided that:
 *  - it's not in the set already
 *  - there's enough space for it
 */
bool sparse_set_add_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem);

/*
 * Remove an arbitrary element from the set.
 * XXX what
 */
SPARSE_SET_ELEMENT_TYPE sparse_set_pop (sparse_set_t *set);

/*
 * Remove the given element from the set, provided that it's already a member.
 */
void sparse_set_remove_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem);

/**
 * Change the maximum size of the set.
 */
void sparse_set_resize (sparse_set_t *set, size_t new_capacity);

/**
 * Destroy and deallocate a set.
 */
void sparse_set_free (sparse_set_t *set);

#ifdef __cplusplus
}
#endif

#endif
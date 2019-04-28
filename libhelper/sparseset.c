#include "sparseset.h"

/**
 * Allocates and initializes a new sparse set, with the given capacity.
 */
sparse_set_t sparse_set_init (size_t capacity) {
    sparse_set_t t = {
        .dense = NULL,
        .sparse = NULL,
        .cardinality = 0,
        .capacity = capacity
    };
    t.dense = malloc (sizeof *t.dense * capacity);
    t.sparse = malloc (sizeof *t.sparse * capacity);

    return t;
}

/**
 * Clears the sparse set.
 * This is a constant time operation.
 */
void sparse_set_clear (sparse_set_t *set) {
    set->cardinality = 0;
}

/**
 * Returns the cardinality of the sparse set.
 */
const size_t sparse_set_size (sparse_set_t *set) {
    return set->cardinality;
}

/**
 * Returns the maximum capacity of the sparse set.
 * Indices can be in the range [0, capacity).
 */
const size_t sparse_set_capacity (sparse_set_t *set) {
    return set->capacity;
}

/*
 * Returns true if elem is a member of the set, false otherwise.
 */
bool sparse_set_is_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem) {
    if (elem < set->capacity) {
        return (unsigned)set->sparse[elem] < set->cardinality && set->dense[set->sparse[elem]] == elem;
    }
    return false;
}

/**
 * Insert the given element in the set, provided that:
 *  - it's not in the set already
 *  - there's enough space for it
 */
bool sparse_set_add_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem) {
    if (!sparse_set_is_member (set, elem) && set->cardinality < set->capacity) {
        size_t index = set->cardinality;
        set->dense[index] = elem;
        set->sparse[elem] = index;
        set->cardinality++;
        return true;
    }
    return false;
}

/*
 * Remove an arbitrary element from the set.
 * XXX what
 */
SPARSE_SET_ELEMENT_TYPE sparse_set_pop (sparse_set_t *set) {
    set->cardinality--;
    return set->dense[set->cardinality + 1];
}

/*
 * Remove the given element from the set, provided that it's already a member.
 */
void sparse_set_remove_member (sparse_set_t *set, SPARSE_SET_ELEMENT_TYPE elem) {
    if (sparse_set_is_member (set, elem)) {
        SPARSE_SET_ELEMENT_TYPE last = sparse_set_pop (set);
        set->dense[set->sparse[elem]] = last;
        set->sparse[last] = set->sparse[elem];
    }
}

/**
 * Change the maximum size of the set.
 */
void sparse_set_resize (sparse_set_t *set, size_t new_capacity) {
    if (set->cardinality > new_capacity) {
        /* Truncate the set. */
        set->cardinality = new_capacity;
        set->capacity = new_capacity;
    }
    if (new_capacity > set->capacity) {
        size_t *temp = NULL;
        if (temp = malloc (sizeof *temp * new_capacity)) {
            if (set->sparse) {
                memmove (temp, set->sparse, set->capacity * sizeof (*temp));
                free (set->sparse);
            }
            set->sparse = temp;
        }

        SPARSE_SET_ELEMENT_TYPE *newstorage = NULL;
        if (newstorage = malloc (sizeof *newstorage * new_capacity)) {
            if (set->dense) {
                memmove (newstorage, set->dense, set->cardinality * sizeof *newstorage);
                free (set->dense);
            }
            set->dense = newstorage;
        }
        set->capacity = new_capacity;
    }
}

/**
 * Destroy and deallocate a set.
 */
void sparse_set_free (sparse_set_t *set) {
    free (set->sparse);
    free (set->dense);
    set->cardinality = set->capacity = 0;
}
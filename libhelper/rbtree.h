#ifndef rbtree_fcf1f8547f3647ed9a40902d4d7bab54
#define rbtree_fcf1f8547f3647ed9a40902d4d7bab54

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rb_node {
    struct rb_node *parent;
    struct rb_node *rb_link[2]; /** Exploit the symmetry here to make the code more concise */
    void *data; /** XXX How about an intrusive version instead? */
    bool red;
};

struct rb_tree {
    struct rb_node *rb_node;
    int (*cmp) (void *, void *); /** Comparator establishing a partial order */
};

#define rb_parent(r)   ((struct rb_node *) (r)->parent)

/**
 * Returns true if the passed node is red, false otherwise.
 */
bool is_red (struct rb_node *node) {
    return node && node->red;
}

/**
 * A single rotation in a red-black tree rotates the nodes and recolors:
 * - the old root is red
 * - the new root is black
 * The direction of the rotation is handled symmetrically.
 */
struct rb_node * rotate_single (struct rb_node *root, bool dir);

struct rb_node * rotate_double (struct rb_node *root, bool dir);

#ifndef NDEBUG
/**
 * Check the invariants of a red-black tree.
 * Returns the black-height of the root node:
 * `cmp` returns -1 if lhs < rhs, 0 if lhs == rhs, 1 if lhs > rhs (a la strcmp)
 */
size_t rb_invariant (struct rb_node *root, int (*cmp)(void *lhs, void *rhs));
#endif

struct rb_node * rb_create_node (void *data);

struct rb_node * rb_insert_node (struct rb_node *root, void *data, int (*cmp)(void *, void *));

struct rb_node * rb_remove_balance (struct rb_node *root, bool right, bool *done);

struct rb_node * rb_remove_node (struct rb_node *root, void *data, int (*cmp)(void *, void *), bool *done);

void rb_remove (struct rb_tree *tree, void *data);

void rb_insert (struct rb_tree *tree, void *data);

#ifdef __cplusplus
}
#endif

#endif

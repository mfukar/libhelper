#ifndef rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54
#define rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54

#include <stdbool.h>
#include <stdint.h>

struct rb_node {
	struct rb_node *parent;
    struct rb_node *rb_link[2]; /** Exploit the symmetry here to make the code more concise */
    void *data; /** XXX How about an intrusive version instead? */
	bool red;
};

struct rb_tree {
	struct rb_node *rb_node;
    int (*cmp)(void *, void *); /** Comparator establishing a partial order */
};

#define rb_parent(r)   ((struct rb_node *) (r)->parent)

static inline bool is_red (struct rb_node *node) {
    return node && node->red;
}

/**
 * A single rotation in a red-black tree rotates the nodes and recolors:
 * - the old root is red
 * - the new root is black
 * The direction of the rotation is handled symmetrically.
 */
static inline struct rb_node *
rotate_single(struct rb_node *root, bool dir) {
    struct rb_node *save = root->rb_link[!dir];

    root->rb_link[!dir] = save->rb_link[dir];
    save->rb_link[dir] = root;

    root->red = true;
    save->red = false;

    return save;
}

static inline struct rb_node *
rotate_double (struct rb_node *root, bool dir) {
    root->rb_link[!dir] = rotate_single (root->rb_link[!dir], !dir);

    return rotate_single (root, dir);
}

/**
 * Check the invariants of a red-black tree.
 * Returns the black-height of the root node:
 * `cmp` returns -1 if lhs < rhs, 0 if lhs == rhs, 1 if lhs > rhs
 */
static inline size_t
rb_invariant (struct rb_node *root, int (*cmp)(void *lhs, void *rhs)) {
#ifndef NDEBUG
    size_t height_left, height_right;

    if (!root) return 1;

    struct rb_node *lh = root->rb_link[0];
    struct rb_node *rh = root->rb_link[1];

    if (is_red (root)
    && (is_red (lh) || is_red (rh))) {
        return 0;
    }

    height_left  = rb_invariant (lh, cmp);
    height_right = rb_invariant (rh, cmp);

    /* A red-black tree is a binary tree. Is this one?
     * We must have:
     *   - lh->data < root->data
     *   - rh->data > root->data
     */
    if (lh && cmp (root->data, lh->data) !=  1
    ||  rh && cmp (root->data, rh->data) != -1) {
        return 0;
    }

    /* Every path from a given node to any of its leaf nodes contains the same number of black nodes: */
    if (lh != 0 && rh != 0 && lh != rh) {
        return 0;
    }

    if (lh != 0 && rh != 0) {
        return is_red (root) ? height_left : height_left + 1;
    }
#endif
    return 0;
}

static inline struct rb_node *
rb_create_node (void *data) {
    struct rb_node *node = malloc (sizeof *node);

    if (!node) {
        return NULL;
    }

    node->data = data;
    node->red = true;
    node->rb_link[0] = NULL;
    node->rb_link[1] = NULL;

    return node;
}

static inline struct rb_node *
rb_insert_node (struct rb_node *root, void *data, int (*cmp)(void *, void *)) {
    if (!root) {
        root = rb_create_node (data);
    }

    if (data != root->data) {
        bool right = cmp (root->data, data) == -1;
        root->rb_link[right] = rb_insert_node (root->rb_link[right], data, cmp);

        /* Now rebalance the tree: */
        if (is_red (root->rb_link[right])) {
            if (is_red (root->rb_link[!right])) {
                root->red = true;
                root->rb_link[0]->red = false;
                root->rb_link[1]->red = false;
            } else {
                if (is_red (root->rb_link[right]->rb_link[right])) {
                    root = rotate_single (root, !right);
                } else {
                    root = rotate_double (root, !right);
                }
            }
        }
    }

    return root;
}

static inline struct rb_node *
rb_remove_node (struct rb_node *root, void *data, bool *done) {
    if (root == NULL) {
        *done = true;
        return root;
    }

    if (root->data == data) {

    }

    bool right = cmp (root->data, data) == -1;

}

static inline void
rb_remove (struct rb_tree *tree, void *data) {
    bool done = false;

    tree->rb_node = rb_remove_node (tree->rb_node, data, &done);
    
    if (tree->rb_node) {
        tree->rb_node->red = false;
    }
}

static inline void
rb_insert (struct rb_tree *tree, void *data) {
    tree->rb_node = rb_insert_node (tree->rb_node, data, tree->cmp);
    tree->rb_node->red = false; /* The root node is always black. */
}



#endif // rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54

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
 * `cmp` returns -1 if lhs < rhs, 0 if lhs == rhs, 1 if lhs > rhs (a la strcmp)
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
    if (lh && cmp (root->data, lh->data) <= 0
    ||  rh && cmp (root->data, rh->data) >= 0) {
        return 0;
    }

    /* Every path from a given node to any of its leaf nodes contains the same number of black nodes: */
    if (height_left != 0 && height_right != 0 && height_left != height_right) {
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
        bool right = cmp (root->data, data) < 0;
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
                } else if (is_red(root->rb_link[right]->rb_link[!right])) {
                    root = rotate_double (root, !right);
                }
            }
        }
    }

    return root;
}

static inline struct rb_node *
rb_remove_balance (struct rb_node *root, bool right, bool *done) {
    struct rb_node *parent = root;
    struct rb_node *sibling = root->rb_link[!right];

    if (sibling && is_red (sibling)) {
        root = rotate_single (root, right);
        sibling = parent->rb_link[!right];
    }

    /* Done! */
    if (sibling == NULL) {
        return root;
    }

    if (!is_red (sibling->rb_link[0]) && !is_red (sibling->rb_link[1])) {
        if (is_red (parent)) {
            *done = true;
        }

        parent->red = true;
        sibling->red = false;
    } else {
        bool color = root->red;
        bool new_root = root == parent;

        if (is_red (sibling->rb_link[!right])) {
            parent = rotate_single (root, right);
        } else {
            parent = rotate_double (root, right);
        }

        parent->red = color;
        parent->rb_link[0]->red = false;
        parent->rb_link[1]->red = false;

        if (new_root) {
            root = parent;
        } else {
            root->rb_link[right] = parent;
        }

        *done = true;
    }

    return root;
}

static inline struct rb_node *
rb_remove_node (struct rb_node *root, void *data, int (*cmp)(void *, void *), bool *done) {
    if (root == NULL) {
        *done = true;
        return root;
    }

    if (root->data == data) {
        if (root->rb_link[0] == NULL || root->rb_link[1] == NULL) {
            struct rb_node *leaf = root->rb_link[root->rb_link[0] == NULL];

            if (is_red (root)) {
                *done = true;
            } else if (is_red(leaf)) {
                leaf->red = false;
                *done = true;
            }

            free (root);

            return leaf;
        } else {
            struct rb_node *heir = root->rb_link[0];

            while (heir->rb_link[1] != NULL) {
                heir = heir->rb_link[1];
            }

            root->data = heir->data;
            data = heir->data;
        }
    }

    bool right = cmp (root->data, data) < 0;

    root->rb_link[right] = rb_remove_node (root->rb_link[right], data, cmp, done);

    if (!*done) {
        root = rb_remove_balance (root, right, done);
    }

    return root;
}

static inline void
rb_remove (struct rb_tree *tree, void *data) {
    bool done = false;

    tree->rb_node = rb_remove_node (tree->rb_node, data, tree->cmp, &done);

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

#ifndef rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54
#define rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54

#include <stdbool.h>
#include <stdint.h>

struct rb_node {
	struct rb_node *parent;
    struct rb_node *rb_link[2]; /** Exploit the symmetry here to make the code more concise. */
	bool red;
};

struct rb_tree {
	struct rb_node *rb_node;
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
 */
static inline size_t
rb_invariant (struct rb_node *root) {
#ifndef NDEBUG
    size_t leftheight, rightheight;

    if (!root) return 1;

    struct rb_node *lh = root->rb_link[0];
    struct rb_node *rh = root->rb_link[1];

    if (is_red (root)) {
        if (is_red (lh) || is_red (rh)) {
            return 0;
        }
    }

    leftheight  = rb_invariant (lh);
    rightheight = rb_invariant (rh);

    if (lh /* && check the keys */
    ||  rh /* && check the keys */) {
        return 0;
    }

    if (lh != 0 && rh != 0 && lh != rh) {
        return 0;
    }

    if (lh != 0 && rh != 0) {
        return is_red (root) ? lh : lh + 1;
    }
#endif
    return 0;
}


#endif // rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54

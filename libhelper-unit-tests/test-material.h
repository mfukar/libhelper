#pragma once

#include <sstream>

int intcmp (void *l, void *r) {
    return *(int *)l - *(int *)r;
}

void inorder (struct rb_node* root, size_t d, std::stringstream & out) {
    if (!root)
        return;

    inorder (root->rb_link[0], d + 1, out);
    out << "Depth: " << d << " " << (root->red ? "red" : "black") << " contains " << *(int *)root->data << std::endl;
    inorder (root->rb_link[1], d + 1, out);
}
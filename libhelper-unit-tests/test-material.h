#pragma once

#include <iostream>

int intcmp (void *l, void *r) {
    return *(int *)l - *(int *)r;
}
/*
void inorder (struct rb_node* root, size_t d) {
    if (!root)
        return;

    inorder (root->rb_link[0], d + 1);
    std::cout << d << " " << root->red ? "red" : "black" << " " << *(int *)root->data << std::endl;
    inorder (root->rb_link[1], d + 1);
}*/
#pragma once

#include <queue>
#include <sstream>

int intcmp (void *l, void *r) {
    return *(int *)l - *(int *)r;
}

void inorder (struct rb_node* root, size_t d, std::stringstream & out) {
    if (!root) return;

    inorder (root->rb_link[0], d + 1, out);
    out << "Depth: " << d << " " << (root->red ? "red" : "black") << " contains " << *(int *)root->data << std::endl;
    inorder (root->rb_link[1], d + 1, out);
}

void levelorder (struct rb_node* root, std::stringstream & out) {
    if (!root) return;

    std::queue<struct rb_node *> q;
    q.push (root);

    unsigned level = 0;
    while (true) {
        int levelsize = q.size ();
        if (levelsize == 0) break;

        out << "Level: " << level << "\n";
        while (levelsize > 0) {
            auto node = q.front ();
            int *datum = (int *)node->data;
            out << "Node [colour: " << node->red << " data: " << *datum << "]\n";
            q.pop ();

            if (node->rb_link[0]) {
                q.push (node->rb_link[0]);
            }
            if (node->rb_link[1]) {
                q.push (node->rb_link[1]);
            }
            --levelsize;
        }
        ++level;
    }
}
#include <stdio.h>

#include "overflow.h"
#include "kmp.h"
#include "aho-corasick.h"
#include "rbtree.h"

struct num {
    size_t val;
    dequeue list;
};

int intcmp (void *l, void *r) {
    return *(int *)l - *(int *)r;
}

void inorder (struct rb_node* root, size_t d) {
    if (!root)
        return;

    inorder (root->rb_link[0], d + 1);
    printf ("[%zu] (%s) %d\n", d, root->red ? "red" : "black", *(int *)root->data);
    inorder (root->rb_link[1], d + 1);
}


int main (int argc, char *argv[]) {
    struct ac_trie trie;
    ac_trie_init (&trie);
    ac_trie_insert (&trie, "she");
    ac_trie_insert (&trie, "tried");
    ac_trie_insert (&trie, "hers");
    ac_trie_insert (&trie, "he");
    ac_trie_insert (&trie, "looking");
    ac_trie_insert (&trie, "what");
    ac_build_failure_function (&trie);
    struct ac_result res = ac_search (&trie, "head of hers, OK?", 
                                      sizeof "head of hers, OK?", 0);
    assert (res.start == 0 && res.end == 2);
    res = ac_search_longest (&trie, "head of hers, OK?",
                             sizeof "head of hers, OK?", 0);
    assert (res.start == 8 && res.end == 12);

    /*======================*/
    int arr[] = { 11, 14, 2, 7, 1, 15, 5, 8};
    struct rb_tree high = { 0 };
    high.cmp = intcmp;
    for (size_t i = 0; i < sizeof arr / sizeof *arr; ++i) {
        rb_insert (&high, &arr[i]);
    }
    inorder (high.rb_node, 0);
    assert (rb_invariant (high.rb_node, high.cmp) == 1);
    rb_remove (&high, &arr[6]);
    inorder (high.rb_node, 0);
    assert (rb_invariant (high.rb_node, high.cmp) == 1);

    return 0;
}

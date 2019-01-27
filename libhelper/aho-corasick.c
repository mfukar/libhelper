#include <aho-corasick.h>

bool ac_state_init (struct ac_state *state) {
    state->id = 0;
    state->depth = 0;
    state->output = 0;
    state->failure = NULL;
    memset (state->gotofunc, 0, sizeof state->gotofunc);
    return true;
}

bool ac_trie_init (struct ac_trie *trie) {
    trie->root = malloc (sizeof *trie->root);
    if (trie->root == NULL) {
        return false;
    }
    trie->nstates = 1;

    ac_state_init (trie->root);
    return true;
}

struct ac_state * ac_state_add (struct ac_state *state, char character) {
    struct ac_state *next = malloc (sizeof *next);

    if (!next) { return NULL; }

    ac_state_init (next);

    next->depth = state->depth + 1;

    state->gotofunc[character] = next;
    return next;
}

bool ac_trie_insert (struct ac_trie *trie, const char *string) {
    if (trie == NULL || string == NULL) {
        return false;
    }
    size_t length = strlen (string);
    size_t index = 0;
    struct ac_state *state = trie->root;
    for (; index < length && ac_next (state, string[index]) != NULL;) {
        state = ac_next (state, string[index]);
        ++index;
    }

    for (; index < length; ++index) {
        struct ac_state *next = ac_state_add (state, string[index]);

        if (ac_next (state, string[index]) != next) { abort (); }

        if (next == NULL) {
            return false;
        }
        /* Assign the ID: */
        next->id = trie->nstates++;

        state = next;
    }

    ac_output (state) = index;
    return true;
}

bool ac_build_failure_function (struct ac_trie *trie) {
    dequeue (q);

    for (size_t idx = 0; idx < ARRAY_SIZE (trie->root->gotofunc); ++idx) {
        if (ac_next (trie->root, idx) == NULL) {
            ac_next (trie->root, idx) = trie->root;
            continue;
        }

        struct ff_queue_element *qe = malloc (sizeof *qe);
        qe->s = ac_next (trie->root, idx);
        dequeue_enqueue (&q, &qe->list);

        qe->s->failure = trie->root;
    }

    for (; !dequeue_is_empty (&q); ) {
        struct ac_state *r = dequeue_data (dequeue_peek (&q), struct ff_queue_element, list)->s;

        for (size_t a = 0; a < ARRAY_SIZE (r->gotofunc); ++a) {
            if (ac_next (r, a) == NULL) {
                continue;
            }

            struct ff_queue_element *qe = malloc (sizeof *qe);
            qe->s = ac_next (r, a);
            dequeue_enqueue (&q, &qe->list);

            struct ac_state *state = ac_fail (r);
            while (ac_next (state, a) == NULL) {
                state = ac_fail (state);
            }
            struct ac_state *new_failure_state = ac_next (state, a);
            qe->s->failure = new_failure_state;
        }

        free (dequeue_data (dequeue_pop_front (&q), struct ff_queue_element, list));
    }

    return true;
}

bool ac_search_matched (struct ac_result res) {
    return !(res.id == 0
          && res.start == -1
          && res.end == -1
          && res.last_state == NULL);
}

struct ac_result ac_search (struct ac_trie *trie, const char *text, size_t ntext, size_t offset) {
    struct ac_state *state = trie->root;
    for (size_t i = 0 + offset; i < ntext; ++i) {
        while (ac_next (state, text[i]) == NULL) {
            state = ac_fail (state);
        }

        state = ac_next (state, text[i]);

        if (ac_output (state) != 0) {
            return (struct ac_result) {
                .id = state->id,
                    .start = i + 1 - ac_output (state),
                    .end = i + 1,
                    .last_state = state
            };
        }
    }

    return (struct ac_result) { .id = 0, .start = -1, .end = -1, .last_state = NULL };
}

/**
 * Scans the entire buffer pointed to by text, and returns the longest match, if found:
 */
struct ac_result ac_search_longest (struct ac_trie *trie, char *text, size_t ntext, size_t offset) {
    struct ac_result result = (struct ac_result) { .start = -1, .end = -1, .last_state = NULL };

    struct ac_state *state = trie->root;
    for (size_t i = offset; i < ntext; ++i) {
        while (ac_next (state, text[i]) == NULL) {
            state = ac_fail (state);
        }

        state = ac_next (state, text[i]);

        if (ac_output (state) != 0) {
            result = (struct ac_result) {
                .id = state->id,
                    .start = i + 1 - ac_output (state),
                    .end = i + 1,
                    .last_state = state
            };
        }
    }

    /* If we've recorded a match, return it: */
    if (result.last_state) {
        return result;
    }
    return (struct ac_result) { .id = 0, .start = -1, .end = -1, .last_state = NULL };
}

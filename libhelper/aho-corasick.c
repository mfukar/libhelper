#include <aho-corasick.h>
#include <aqueue.h>

/* Implementation of Aho-Corasick search, as per http://cr.yp.to/bib/1975/aho.pdf */

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

    return ac_state_init (trie->root);
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

        if (ac_next (state, string[index]) != next) { abort (); } /* XXX: Do not abort */

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

/**
 * You can cross-ref the comments with the paper.
 */
bool ac_build_failure_function (struct ac_trie *trie) {
    int start_index = 0, end_index = 0;
    struct ac_state **queue = malloc(sizeof *queue * trie->nstates);
    if (!queue) {
        return false;
    }
    SGLIB_QUEUE_INIT (struct ac_state *, queue, start_index, end_index);

    trie->root->failure = trie->root;

    for (size_t a = 0; a < ARRAY_SIZE (trie->root->gotofunc); ++a) {
        /* For root (0), it must be that g(0, a) != 'fail' (NULL) */
        if (ac_next (trie->root, a) == NULL) {
            ac_next (trie->root, a) = trie->root;
            continue;
        }
        /* The transitions to non-NULL nodes are going to be used for BFS: */
        SGLIB_QUEUE_ADD (struct ac_state *, queue, ac_next (trie->root, a), start_index, end_index, trie->nstates);

        /* f(s) = 0 for all states of depth 1 */
        ac_next(trie->root, a)->failure = trie->root;
    }
 
    while (!SGLIB_QUEUE_IS_EMPTY (struct ac_state *, queue, start_index, end_index)) {
        struct ac_state *r = SGLIB_QUEUE_FIRST_ELEMENT (struct ac_state *, queue, start_index, end_index);

        for (size_t a = 0; a < ARRAY_SIZE (r->gotofunc); ++a) {
            if (ac_next (r, a) == NULL) {
                continue;
            }

            /* For each a, such that g(r, a) = s */
            struct ac_state *s = ac_next (r, a);
            /* Enqueue s for the BFS: */
            SGLIB_QUEUE_ADD (struct ac_state *, queue, s, start_index, end_index, trie->nstates);

            /* Set state = f(r) */
            struct ac_state *state = ac_fail (r);
            /* Until g(state, a) != fail */
            while (ac_next (state, a) == NULL) {
                /* execute the statement state = f(state)*/
                state = ac_fail (state);
            }
            /* Set f(s) = g(state, a) */
            ac_fail(s) = ac_next (state, a);
        }

        SGLIB_QUEUE_DELETE (struct ac_state *, queue, start_index, end_index, trie->nstates);
    }

    free (queue);
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

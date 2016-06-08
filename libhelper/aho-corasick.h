#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "queue.h"

#define ARRAY_SIZE(a) sizeof (a) / sizeof (*a)

struct ac_state {
	size_t id;
	size_t depth;
	size_t output;
	struct ac_state *failure;
	struct ac_state *gotofunc[256];
};

struct ac_trie {
	size_t nstates;
	struct ac_state *root;
};

#define ac_depth(s) (s)->depth
#define ac_next(s, a) (s)->gotofunc[(a)]
#define ac_fail(s) (s)->failure
#define ac_output(s) (s)->output

static inline bool
ac_state_init (struct ac_state *state) {
	state->id = 0;
	state->depth = 0;
	state->output = 0;
	state->failure = NULL;
	memset (state->gotofunc, 0, sizeof state->gotofunc);
	return true;
}

static inline bool
ac_trie_init (struct ac_trie *trie) {
	trie->root = malloc (sizeof *trie->root);
	if (trie->root == NULL) {
		return false;
	}
	trie->nstates = 1;

	ac_state_init (trie->root);
	return true;
}

static inline struct ac_state *
ac_state_add (struct ac_state *state, char character) {
	struct ac_state *next = malloc (sizeof *next);

	if (!next) { return NULL; }

	ac_state_init (next);

	next->depth = state->depth + 1;

	state->gotofunc[character] = next;
	return next;
}

static inline bool
ac_trie_insert (struct ac_trie *trie, char *string) {
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

typedef struct {
	struct ac_state *s;
	dequeue list;
} queue_element;

static inline bool
ac_build_failure_function (struct ac_trie *trie) {
	dequeue (q);

	for (size_t idx = 0; idx < ARRAY_SIZE(trie->root->gotofunc); ++idx) {
		if (trie->root->gotofunc[idx] == NULL) {
			ac_next (trie->root, idx) = trie->root;
			continue;
		}

		queue_element *qe = malloc (sizeof *qe);
		qe->s = ac_next (trie->root, idx);
		dequeue_enqueue (&q, &qe->list);

		qe->s->failure = trie->root;
	}

	for (; !dequeue_is_empty (&q); ) {
		struct ac_state *r = dequeue_data (dequeue_peek (&q), queue_element, list)->s;

		for (size_t a = 0; a < ARRAY_SIZE(r->gotofunc); ++a) {
			if (ac_next(r, a) == NULL) {
				continue;
			}

			queue_element *qe = malloc (sizeof *qe);
			qe->s = ac_next (r, a);
			dequeue_enqueue (&q, &qe->list);

			struct ac_state *state = ac_fail (r);
			while (ac_next(state, a) == NULL) {
				state = ac_fail (state);
			}
			struct ac_state *new_failure_state = ac_next (state, a);
			qe->s->failure = new_failure_state;
		}

		free (dequeue_data(dequeue_pop_front (&q), queue_element, list));
	}

	return true;
}

typedef struct {
	size_t id;
	size_t start;
	size_t end;
	struct ac_state *last_state;
} ac_result;

ac_result ac_search (struct ac_trie *trie, char *text, size_t ntext, size_t offset) {
	struct ac_state *state = trie->root;
	for (size_t i = 0 + offset; i < ntext; ++i) {
		while (ac_next (state, text[i]) == NULL) {
			state = ac_fail (state);
		}

		state = ac_next (state, text[i]);

		if (ac_output (state) != 0) {
			return (ac_result) { .id = state->id,
				                 .start = i + 1 - ac_output(state),
								 .end = i + 1,
				                 .last_state = state };
		}
	}

	return (ac_result) {.id = 0, .start = -1, .end = -1, .last_state = NULL };
}

/**
 * Scans the entire buffer pointed to by text, and returns the longest match, if found:
 */
ac_result ac_search_longest (struct ac_trie *trie, char *text, size_t ntext, size_t offset) {
	ac_result result = (ac_result) { .start = -1, .end = -1, .last_state = NULL };

	struct ac_state *state = trie->root;
	for (size_t i = offset; i < ntext; ++i) {
		while (ac_next (state, text[i]) == NULL) {
			state = ac_fail (state);
		}

		state = ac_next (state, text[i]);

		if (ac_output (state) != 0) {
			result = (ac_result) { .id = state->id,
					               .start = i + 1 - ac_output (state),
                                   .end = i + 1,
					               .last_state = state };
		}
	}

    /* If we've recorded a match, return it: */
    if (result.last_state) {
        return result;
    }
	return (ac_result) { .id = 0, .start = -1, .end = -1, .last_state = NULL };
}

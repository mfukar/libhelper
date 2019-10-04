#ifndef ac_6c7055133686455382b48f901be5abc6
#define ac_6c7055133686455382b48f901be5abc6

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ac_state {
    bool terminal;
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

/**
 * Must be called before `ac_build_failure_function`, `ac_search`, or `ac_search_longest`
 */
bool ac_trie_init (struct ac_trie *trie);

/**
 * Insert a `string` into the `trie`.
 * Returns `true` if insertion was successful, or `false` on error.
 */
bool ac_trie_insert (struct ac_trie *trie, const char *string);

/**
 * Must be called before `ac_search` or `ac_search_longest`
 */
bool ac_build_failure_function (struct ac_trie *trie);

struct ac_result {
	size_t id;
	size_t start;
	size_t end;
	struct ac_state *last_state;
};

/**
 * Helper function to determine whether a match was found. Will return true
 * if it was, false otherwise.
 */
bool ac_search_matched (struct ac_result res);

struct ac_result ac_search (struct ac_trie *trie, const char *text, size_t ntext, size_t offset);

/**
 * Scans the entire buffer pointed to by text, and returns the longest match, if found.
 */
struct ac_result ac_search_longest (struct ac_trie *trie, char *text, size_t ntext, size_t offset);

#ifdef __cplusplus
}
#endif

#endif

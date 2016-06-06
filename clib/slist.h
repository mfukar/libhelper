#ifndef MF_SLIST_H
#define MF_SLIST_H

#include <stdlib.h>

#ifdef __cplusplus__
extern "C" {
#endif

#define slist_car(l) slist_get_pair(l, 0)
#define slist_cdr(l) slist_get_tail(l, 0)
#define slist_caar(l) slist_car(slist_car(l))
#define slist_cadr(l) slist_car(slist_cdr(l))
#define slist_cdar(l) slist_cdr(slist_car(l))
#define slist_cddr(l) slist_cdr(slist_cdr(l))

/**
 * Implementation of a singly-linked tail-queue, loosely based on Lisp lists.
 */
struct sl_pair {
	void *head;
	struct sl_pair *tail;
};

/**
 * Insidious HACK:
 */
typedef struct sl_pair *slist;

#define slist_head(pair) (pair)->head
#define slist_tail(pair) (pair)->tail

static inline void slist_free(slist list) {
	slist it;

	while (list != NULL) {
		it = list;
		list = slist_tail(list);
		free(it);
	}
}

static inline slist slist_cons (slist tail, void *head) {
	slist sl = malloc(sizeof *sl);
	slist_head(sl) = head;
	slist_tail(sl) = tail;
	return sl;
}

static inline void * slist_get_pair (slist pair, size_t index) {
	for (size_t i = 0; pair && i != index; ++i) {
		pair = slist_tail(pair);
	}
	
	if (pair == NULL) {
        return NULL;
    }

	return slist_head(pair);
}

static inline slist slist_get_tail (slist pair, size_t index) {
    for (size_t i = 0; pair && i != index; ++i) {
        pair = slist_tail (pair);
    }

    if (pair == NULL) {
        return NULL;
    }

    return slist_tail (pair);
}

static inline slist slist_push(slist *list, void *data) {
	return *list = slist_cons(*list, data);
}

static inline slist slist_push_pair(slist *list, slist pair) {
	slist_tail (pair) = *list;
	return *list = pair;
}

static inline slist slist_pop_pair (slist *list) {
	slist result = *list;

    if (result == NULL) {
        return result;
    }
	*list = slist_tail (result);
    return result;
}

static inline void * slist_pop(slist *list) {
	slist pair = slist_pop_pair (list);

	if (pair == NULL) {
		return pair;
	}
    
    void *item = slist_head (pair);
    free (pair);
    return item;
}

/**
 * Reverse a list without constructing a new one ("nreverse"):
 */
static inline slist slist_reverse_inplace (slist list) {
    slist result = NULL;

    while (list != NULL) {
        slist_push_pair (&result, slist_pop_pair (&list));
    }
    return result;
}

/**
 * Construct and return a new list, which is the reverse of the supplied one:
 */
static inline slist slist_reverse (slist list) {
    slist result = NULL;
    slist next = list;

    while (next != NULL) {
        void * head = slist_head (next);
        result = slist_cons (result, head);
        next = slist_tail (next);
    }
    return result;
}

/**
 * Merge two unordered lists based on the predicate in `cmp`:
 */
static inline slist slist_merge (slist list1, slist list2, int (*cmp)(void *, void *)) {
    slist result = NULL;

    while (list1 != NULL && list2 != NULL) {
        slist_push_pair (&result,
                         cmp (slist_head (list2), slist_head (list1))
                         ? slist_pop_pair (&list1)
                         : slist_pop_pair (&list2));
    }
    while (list1 != NULL) {
        slist_push_pair (&result, slist_pop_pair (&list1));
    }
    while (list2 != NULL) {
        slist_push_pair (&result, slist_pop_pair (&list2));
    }
    return slist_reverse_inplace (result);
}


static inline slist slist_split (slist list) {
    if (list == NULL || slist_tail(list)) {
        return list;
    }

    slist tail = slist_tail (list);

    while (tail != NULL && (tail = slist_tail(tail)) != NULL) {
        list = slist_tail (list);
        tail = slist_tail (tail);
    }

    tail = slist_tail (list);
    slist_tail (list) = NULL;
    return tail;
}

/**
 * Sort a list recursively in-place:
 */
static inline slist slist_sort (slist list, int (*cmp)(void *, void*)) {
    if (list == NULL || slist_tail (list) == NULL) {
        return list;
    }

    slist result = slist_split(list);
    return slist_merge (slist_sort(list, cmp), slist_sort(result, cmp), cmp);
}

#ifdef __cplusplus__
}
#endif
#endif /* MF_SLIST_H */
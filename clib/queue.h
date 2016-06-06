#pragma once

#include "dlist.h"

#ifdef __cplusplus__
extern "C" {
#endif

typedef struct dlist dequeue;

#define dequeue(N) dequeue N = DLIST_HEAD_INIT(N)
#define dequeue_reset dlist_init
#define dequeue_is_empty dlist_is_empty
#define dequeue_data dlist_entry
#define dequeue_peek dlist_get_prev
#define dequeue_enqueue dlist_prepend

static inline dequeue *
dequeue_pop_front (dequeue *queue) {
	dequeue *node = dlist_get_prev (queue);
	dlist_delete (node);
	return node;
}

#ifdef __cplusplus__
}
#endif

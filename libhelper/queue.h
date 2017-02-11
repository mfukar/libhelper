#ifndef queue_2e4f3b0fa3304dd990755cfa2c99cab1
#define queue_2e4f3b0fa3304dd990755cfa2c99cab1

#include "dlist.h"

#ifdef __cplusplus
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

#ifdef __cplusplus
}
#endif

#endif
#pragma once
#include "dlist.h"

/**
 * Allocate a double-ended (intrusive) queue. Defines the head of the list.
 * `dequeue_init` must be called on `NAME` before any other operation.
 */
#define dequeue_alloc(NAME) struct dlist_node (NAME) = DLIST_HEAD_INIT(NAME);
 
 /**
  * Initialise an empty double-ended queue. The queue must have been defined with
  * `dequeue_alloc` before. Must be called before any other operation.
  * @queue: A pointer to a dequeue
  */
#define dequeue_init(queue) dlist_init(queue)

/**
 * Find if a double-ended queue is empty. Returns `true` if the queue is empty, `false` otherwise.
 */
static inline bool dequeue_is_empty (struct dlist_node *queue) {
    return queue->next == queue;
}

/**
 * Get the element (struct) which contains `cursor`. Uses `container_of`.
 * @cursor: a pointer to a `dequeue` member of the object needed
 * @type: the type of the object containing `ptr`
 * @member_name: the name of the `dequeue` member of the object needed
 */
#define dequeue_data(cursor, type, member_name) dlist_entry(cursor, type, member_name)

/**
 * Return a pointer to a `dequeue` pointing to the back of the double-ended queue.
 * The element at that location can be retrieved with `dequeue_data (dequeue_front(queue), TYPE, MEMBER_NAME)`
 */
#define dequeue_front(queue) dlist_get_prev(queue)

/**
 * Enqueue an element in the double-ended queue.
 */
#define dequeue_enqueue(queue, element) dlist_prepend(queue, element)
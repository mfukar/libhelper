#ifndef dlist_959d884c829d4e6ca0462bf53af52923 
#define dlist_959d884c829d4e6ca0462bf53af52923

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Intrusive double-linked list.
 * Based on the Linux kernel implementation
 * in /source/tools/include/linux/list.h
 */

struct dlist {
    struct dlist *next, *prev;
};

/* Initializer macros: */
#define DLIST_HEAD_INIT(name) { &(name), &(name) }
#define DLIST_HEAD(name) struct dlist name = DLIST_HEAD_INIT(name)

/**
 * dlist_entry - get the dlist element for this entry
 * @ptr:        a pointer to the `struct dlist`
 * @type:       the type of the struct this is embedded in
 * @member:     the name of the `struct dlist *` within the element
 */
#define dlist_entry(ptr, type, member) container_of(ptr, type, member)

#define dlist_get_next(ptr) (ptr)->next
#define dlist_get_prev(ptr) (ptr)->prev

#define dlist_for_each(head, pos) \
    for ((pos) = (head)->next;    \
         (pos) != (head);         \
         (pos) = (pos)->next)

#define dlist_for_each_safe(head, pos, tmp)         \
    for ((pos) = (head)->next, (tmp) = (pos)->next; \
         (pos) != (head);                           \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * Iterate over a dlist of given type. Not safe against mutation during iteration.
 * @head:       the head of the dlist
 * @pos:        a pointer to the element type, to use as a loop cursor
 * @member:     the name of the `struct dlist *` within the element
 */
#define dlist_for_each_entry_unsafe(head, pos, member) \
    for (pos = dlist_entry((head)->next, typeof(*pos), member);\
         &pos->member != (head);\
         pos = dlist_entry(pos->member.next, typeof(*pos), member))

/**
 * Iterate over a dlist of given type, backwards. Not safe against mutation during iteration.
 * @head:       the head of the dlist
 * @pos:        a pointer to the element type, to use as a loop cursor
 * @member:     the name of the `struct dlist *` within the element
 */
#define dlist_for_each_previous_entry_unsafe(head, pos, member) \
    for (pos = dlist_entry((head)->prev, typeof(*pos), member);\
         &pos->member != (head);\
         pos = dlist_entry(pos->member.prev, typeof(*pos), member))

/**
 * Iterate over a dlist of given type. It is safe to delete the element under the
 * cursor when using this macro.
 * @head: the head of the dlist
 * @pos: a pointer to the element type, to use as a loop cursor
 * @tmp: a pointer to the element type, to use as scratch
 * @member: the name of the `struct dlist *` within the element
 */
#define dlist_for_each_entry_safe(head, pos, tmp, member) \
    for (pos = dlist_entry((head)->next, typeof(*pos), member),     \
           n = dlist_entry(pos->member.next, typeof(*pos), member); \
        &pos->member != (head);                                    \
        pos = n, n = dlist_entry(n->member.next, typeof(*n), member))

/**
 * Iterate over a dlist of given type, backwards. It is safe to delete the
 * element under the cursor when using this macro.
 * @head: the head of the dlist
 * @pos: a pointer to the element type, to use as a loop cursor
 * @tmp: a pointer to the element type, to use as scratch
 * @member: the name of the `struct dlist *` within the element
 */
#define dlist_for_each_previous_entry_safe(head, pos, tmp, member) \
    for (pos = dlist_entry((head)->prev, typeof(*pos), member),     \
           n = dlist_entry(pos->member.prev, typeof(*pos), member); \
        &pos->member != (head);                                    \
        pos = n, n = dlist_entry(n->member.prev, typeof(*n), member))


static inline void dlist_init(struct dlist *list) {
    list->next = list;
    list->prev = list;
}

static inline bool dlist_is_empty(struct dlist *list) {
    return list->next == list;
}

static inline bool dlist_is_last(struct dlist *entry, struct dlist *head) {
    return entry->next == head;
}

/**
 * For internal usage only.
 * Insert a new element between two known, consecutive elements:
 */
static inline void _dlist_insert(struct dlist *elem, struct dlist *prev, struct dlist *next) {
    next->prev = elem;
    elem->next = next;
    elem->prev = prev;
    prev->next = elem;
}

static inline void dlist_enqueue(struct dlist *head, struct dlist *elem) {
    _dlist_insert(elem, head->prev, head);
}

static inline void dlist_prepend(struct dlist *head, struct dlist *elem) {
    _dlist_insert(elem, head, head->next);
}

/**
 * For internal usage only.
 * Delete an element by linking its `prev` and `next` pointers.
 */
static inline void _dlist_delete(struct dlist *prev, struct dlist *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void dlist_delete(struct dlist *entry) {
    _dlist_delete(entry->prev, entry->next);
    entry->next = entry;
    entry->prev = entry;
}

static inline void dlist_replace(struct dlist *old_entry, struct dlist *new_entry) {
    new_entry->next = old_entry->next;
    new_entry->next->prev = new_entry;
    new_entry->prev = old_entry->prev;
    new_entry->prev->next = new_entry;
}

static inline void dlist_merge(struct dlist *head, struct dlist *list) {
    struct dlist *first = list->next;
    struct dlist *last = list->prev;
    struct dlist *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

#ifdef __cplusplus
}
#endif

#endif

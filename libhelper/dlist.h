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

/**
 * Definition of a list node. The list head has the same type, so take
 * care to distinguish its name.
 *
 * To define the list head:
 * struct f {
 *     ...
 *     struct dlist_node head_of_list_of_things;
 * };
 *
 * struct g {
 *     struct dlist_node entry;
 * };
 */
struct dlist_node {
    struct dlist_node *next, *prev;
};

/**
 * Initializer macros:
 */
#define DLIST_HEAD_INIT(name) { &(name), &(name) }
#define DLIST_HEAD(name) struct dlist_node name = { &(name), &(name) }

/**
 * Initialise a list head. Useful when allocating dynamically.
 */
static inline void dlist_init (struct dlist_node *list) {
    list->next = list;
    list->prev = list;
}

/**
 * dlist_entry - get the element which contains this dlist node
 * @ptr:        a pointer to the `struct dlist`
 * @type:       the type of the struct this is embedded in
 * @member:     the name of the `struct dlist *` field within the structure
 */
#define dlist_entry(ptr, type, member) container_of(ptr, type, member)

#define dlist_get_next(ptr) (ptr)->next
#define dlist_get_prev(ptr) (ptr)->prev

static inline bool dlist_is_empty(struct dlist_node *list) {
    return list->next == list;
}

static inline bool dlist_is_last(struct dlist_node *entry, struct dlist_node *head) {
    return entry->next == head;
}

/**
 * For internal usage only.
 * Insert a new element between two known, consecutive elements:
 */
static inline void _dlist_insert(struct dlist_node *elem, struct dlist_node *prev, struct dlist_node *next) {
    next->prev = elem;
    elem->next = next;
    elem->prev = prev;
    prev->next = elem;
}

/**
 * Add an element to the end of the list.
 * Example:
 * struct f *item = malloc(sizeof *item);
 *     ...
 * dlist_append(&list_of_items, &item->entry);
 */
static inline void dlist_append(struct dlist_node *head, struct dlist_node *elem) {
    _dlist_insert(elem, head->prev, head);
}

/**
 * Add an element to the end of the list.
 * Example:
 * struct f *item = malloc(sizeof *item);
 *     ...
 * dlist_prepend(&list_of_items, &item->entry);
 */
static inline void dlist_prepend(struct dlist_node *head, struct dlist_node *elem) {
    _dlist_insert(elem, head, head->next);
}

/**
 * For internal usage only.
 * Delete an element by linking its `prev` and `next` pointers.
 */
static inline void _dlist_delete(struct dlist_node *prev, struct dlist_node *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void dlist_delete(struct dlist_node *entry) {
    _dlist_delete(entry->prev, entry->next);
    entry->next = entry;
    entry->prev = entry;
}

static inline void dlist_replace(struct dlist_node *old_entry, struct dlist_node *new_entry) {
    new_entry->next = old_entry->next;
    new_entry->next->prev = new_entry;
    new_entry->prev = old_entry->prev;
    new_entry->prev->next = new_entry;
}

static inline void dlist_merge(struct dlist_node *head, struct dlist_node *list) {
    struct dlist_node *first = list->next;
    struct dlist_node *last = list->prev;
    struct dlist_node *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

#define dlist_for_each(head, pos) \
    for ((pos) = (head)->next; \
         (pos) != (head); \
         (pos) = (pos)->next)

/**
 * Iterate over a dlist. Safe against mutation during iteration.
 * @head:       the head of the dlist
 * @cursor:     a pointer to the struct dlist, to use for mutation of the list
 * @constant:   a pointer to a second struct dlist, to use as a loop cursor
 */
#define dlist_for_each_safe(head, cursor, constant) \
    for ((cursor) = (head)->next, (constant) = (cursor)->next; \
         (cursor) != (head); \
         (cursor) = (constant), (constant) = (constant)->next)

/**
 * Iterate over the elements of a dlist, of given type. Not safe against mutation during iteration.
 * @head:       the head of the dlist
 * @pos:        a pointer to the element type, to use as a loop cursor
 * @member:     the name of the `struct dlist *` within the element
 */
#define dlist_for_each_entry_unsafe(head, pos, member) \
    for (pos = dlist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = dlist_entry(pos->member.next, typeof(*pos), member))

 /**
  * Iterate over the elements of a dlist, of given type, backwards. Not safe against mutation during iteration.
  * @head:       the head of the dlist
  * @pos:        a pointer to the element type, to use as a loop cursor
  * @member:     the name of the `struct dlist *` within the element
  */
#define dlist_for_each_previous_entry_unsafe(head, pos, member) \
    for (pos = dlist_entry((head)->prev, typeof(*pos), member); \
         &pos->member != (head); \
         pos = dlist_entry(pos->member.prev, typeof(*pos), member))

  /**
   * Iterate over a dlist of given type. It is safe to delete the element under the
   * cursor when using this macro.
   * @head: the head of the dlist
   * @cursor:   a pointer to the element type, to use for mutation of the list
   * @constant: a pointer to the element type, to use as a loop cursor
   * @member: the name of the `struct dlist *` within the element
   */
#define dlist_for_each_entry_safe(head, cursor, constant, member) \
    for (cursor = dlist_entry((head)->next, typeof(*cursor), member), \
         constant = dlist_entry(cursor->member.next, typeof(*cursor), member); \
        &cursor->member != (head); \
        cursor = constant, constant = dlist_entry(constant->member.next, typeof(*constant), member))

   /**
    * Iterate over a dlist of given type, backwards. It is safe to delete the
    * element under the cursor when using this macro.
    * @head: the head of the dlist
    * @cursor: a pointer to the element type, to use for mutation of the list
    * @constant: a pointer to the element type, to use as a loop cursor
    * @member: the name of the `struct dlist *` within the element
    */
#define dlist_for_each_previous_entry_safe(head, cursor, constant, member) \
    for (cursor = dlist_entry((head)->prev, typeof(*cursor), member), \
         constant = dlist_entry(cursor->member.prev, typeof(*cursor), member); \
        &cursor->member != (head); \
        cursor = constant, constant = dlist_entry(constant->member.prev, typeof(*constant), member))

#ifdef __cplusplus
}
#endif

#endif

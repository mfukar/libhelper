#ifndef aqueue_2e4f3b0fa3304dd990755cfa2c99cab1
#define aqueue_2e4f3b0fa3304dd990755cfa2c99cab1

#ifdef __cplusplus
extern "C" {
#endif

/* A simple array-backed queue (FIFO) macro API. Useful when neither performance
 * or storage size matter, e.g. because it's a one-time cost. Allocate an
 * array of sufficient size before you use it with:
 * `AQUEUE_INIT(...);`
 * `AQUEUE_ADD(...);`
 *
 * The queue is a quadruple (array, index_used, index_free, size) such that
 *  `array` is the array storing objects
 *  `index_used` is the index of the first used element in the array
 *  `index_free` is the index of the first free element in the array
 *  `size` is (sizeof array / sizeof *array), i.e. array capacity
 */
#define AQUEUE_INIT(array, index_used, index_free) { index_used = index_free = 0; }

#define AQUEUE_IS_EMPTY(array, index_used, index_free) ((index_used) == (index_free))

#define AQUEUE_IS_FULL(array, index_used, index_free, dim) ((index_used) == ((index_free) + 1) % (dim))

#define AQUEUE_FRONT_ELEMENT(array, index_used, index_free) (array[index_used])

#define __queue_add_back(array, index_used, index_free, dim) { if (!AQUEUE_IS_FULL(array, index_used, index_free, dim)) (index_free) = ((index_free) + 1) % (dim); }
#define AQUEUE_ENQUEUE(array, elem, index_used, index_free, dim) { array[index_free] = (elem); __queue_add_back(array, index_used, index_free, dim); }

#define AQUEUE_DEQUEUE(array, index_used, index_free, dim) { if (!AQUEUE_IS_EMPTY(array, index_used, index_free)) { (index_used) = ((index_used) + 1) % (dim); } }

#ifdef __cplusplus
}
#endif

#endif
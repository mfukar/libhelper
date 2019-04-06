#ifndef queue_2e4f3b0fa3304dd990755cfa2c99cab1
#define queue_2e4f3b0fa3304dd990755cfa2c99cab1

#ifdef __cplusplus
extern "C" {
#endif

/* A simple array-backed queue macro API. Useful when neither performance
 * or storage size matter, e.g. because it's a one-time cost. Allocate an
 * array of sufficient size before you use it with:
 * `SGLIB_QUEUE_INIT(...);`
 * `SGLIB_QUEUE_ADD(...);`
 *
 * The queue is a quadruple (array, i, j, size) such that
 *  `array` is the array storing values
 *  `i` is the index of the first used element in the array
 *  `j` is the index of the first free element in the array
 *  `size` is the array capacity
 */
#define SGLIB_QUEUE_INIT(type, a, i, j) { i = j = 0; }
#define SGLIB_QUEUE_IS_EMPTY(type, a, i, j) ((i)==(j))
#define SGLIB_QUEUE_IS_FULL(type, a, i, j, dim) ((i)==((j)+1)%(dim))
#define SGLIB_QUEUE_FIRST_ELEMENT(type, a, i, j) (a[i])
#define __queue_add_next(type, a, i, j, dim) {\
  if (!SGLIB_QUEUE_IS_FULL(type, a, i, j, dim))\
  (j) = ((j)+1) % (dim);\
}
#define SGLIB_QUEUE_ADD(type, a, elem, i, j, dim) {\
  a[j] = (elem);\
  __queue_add_next(type, a, i, j, dim);\
}
#define SGLIB_QUEUE_DELETE(type, a, i, j, dim) {\
  if (!SGLIB_QUEUE_IS_EMPTY(type, a, i, j))\
  (i) = ((i)+1) % (dim);\
}

#ifdef __cplusplus
}
#endif

#endif
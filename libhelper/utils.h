#ifndef utils_08B13F277C7649A9ACB26C015786D5A7
#define utils_08B13F277C7649A9ACB26C015786D5A7

#define ARRAY_SIZE(a) sizeof (a) / sizeof (*a)

/* Portable implementation of container_of: */
#define container_of(ptr, type, member) \
    ((type *) ((char *)(ptr) - offsetof(type, member)))

#endif

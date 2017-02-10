#ifndef utils_08b13f277c7649a9acb26c015786d5a7
#define utils_08b13f277c7649a9acb26c015786d5a7

#define ARRAY_SIZE(a) sizeof (a) / sizeof (*a)

/* Portable implementation of container_of: */
#define container_of(ptr, type, member) \
    ((type *) ((char *)(ptr) - offsetof(type, member)))

#endif

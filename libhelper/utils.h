#ifndef utils_08b13f277c7649a9acb26c015786d5a7
#define utils_08b13f277c7649a9acb26c015786d5a7

#define ARRAY_SIZE(a) sizeof (a) / sizeof (*a)

/**
 * Portable implementation of container_of.
 * @ptr: a pointer to a member of the object (struct) needed
 * @type: the type of the object containing `ptr`
 * @member_name: the name of the member contained in the object needed
 */
#define container_of(ptr, type, member_name) \
    ((type *) ((char *)(ptr) - offsetof(type, member_name)))

#endif

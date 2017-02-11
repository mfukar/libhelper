#ifndef assert_ab860c1d54014aaa9428fba0fed60ea6
#define assert_ab860c1d54014aaa9428fba0fed60ea6

#ifdef __cplusplus
extern "C" {
#endif

#define mf_assert(x)                                                  \
    do {                                                              \
        if (!(x)) {                                                   \
            fprintf(stderr, "Assertion failed: %s (%s:%s:%d)\n", # x, \
                    __func__, __FILE__, __LINE__);                    \
            fflush(stderr);                                           \
            abort();                                                  \
        }                                                             \
    } while (0)

#define mf_assert_alloc(x)                                \
    do {                                                  \
        if (!(x)) {                                       \
            fprintf(stderr, "Out of memory (%s:%s:%d)\n", \
                    __func__, __FILE__, __LINE__);        \
            fflush(stderr);                               \
            abort();                                      \
        }                                                 \
    } while (0)

#define mf_assert_fmt(x, fmt, ...)                                       \
    do {                                                                 \
        if (!(x)) {                                                      \
            fprintf(stderr, "Assertion failed: %s (%s:%s:%d) " fmt "\n", \
                    # x, __func__, __FILE__, __LINE__, __VA_ARGS__);     \
            fflush(stderr);                                              \
            abort();                                                     \
        }                                                                \
    } while (0)

#define mf_errno_assert(x)                          \
    do {                                            \
        if (!(x)) {                                 \
            fprintf(stderr, "%s [%d] (%s:%s:%d)\n", \
                    strerror(errno), errno,         \
                    __func__, __FILE__, __LINE__);  \
            fflush(stderr);                         \
            abort();                                \
        }                                           \
    } while (0)

#ifdef __cplusplus
}
#endif
#endif

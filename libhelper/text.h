#ifndef text_898722CF5BC54ff08A0376C0CFED8E04
#define text_898722CF5BC54ff08A0376C0CFED8E04

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Strip leading and trailing whitespace from a string.
 * `from` is a pointer to allocated memory holding a NUL-terminated string.
 * `to` is a pointer to allocated `ndst` bytes, which will be NUL-terminated
 * Returns true if any characters were stripped, false otherwise.
 */
bool strip (char * dst, const char * src, size_t ndst);

#ifdef __cplusplus
}
#endif
#endif
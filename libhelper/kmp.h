#ifndef kmp_37b632a1d8a24097b7e85fab69ffefe7
#define kmp_37b632a1d8a24097b7e85fab69ffefe7

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 */
size_t kmp (const char * haystack, size_t hlen, const char * needle, size_t nlen, size_t * ff);

/**
 *
 */
void kmp_ff (const char * needle, size_t * ff, size_t nff);

#ifdef __cplusplus
}
#endif

#endif

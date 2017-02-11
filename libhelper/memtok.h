#ifndef memtok_3b849c5f87bd41179e328e606f06da9c
#define memtok_3b849c5f87bd41179e328e606f06da9c

#include "bytes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse a memory buffer into a sequence of tokens.
 * 
 * The first call to memtok has a non-null first argument; subsequent calls
 * have a null first argument.
 */
bytes_t memtok (const void * restrict s, size_t length, const char * restrict delim, bytes_t * restrict save_ptr);

#ifdef __cplusplus
}
#endif

#endif

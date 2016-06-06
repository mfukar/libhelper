#ifndef overflow_86C759C7_67F9_4242_9A14_73C63A3545B0
#define overflow_86C759C7_67F9_4242_9A14_73C63A3545B0

#include "stdafx.h"

/**
* @description Performs 64-bit two's complement addition of A and B and stores the result
* into *RP
* @return true if overflow occurred, otherwise false.
*/
static inline bool checked_add_64 (int64_t a, int64_t b, int64_t *rp) {
	int64_t r = (uint64_t)a + (uint64_t)b;
	*rp = r;
	return (a < 0 && b < 0 && r >= 0)
		|| (a >= 0 && b >= 0 && r < 0);
}

/**
* @description Performs 32-bit two's complement addition of A and B and stores the result
* into *RP
* @return true if overflow occurred, otherwise false.
*/
static inline bool checked_add_32 (int32_t a, int32_t b, int32_t *rp) {
	int32_t r = (uint32_t)a + (uint32_t)b;
	*rp = r;
	return (a < 0 && b < 0 && r >= 0)
		|| (a >= 0 && b >= 0 && r < 0);
}

/**
* @description Performs 16-bit two's complement addition of A and B and stores the result
* into *RP
* @return true if overflow occurred, otherwise false.
*/
static inline bool checked_add_16 (int16_t a, int16_t b, int16_t *rp) {
	int16_t r = (uint16_t)a + (uint16_t)b;
	*rp = r;
	return (a < 0 && b < 0 && r >= 0)
		|| (a >= 0 && b >= 0 && r < 0);
}

#endif // overflow_86C759C7_67F9_4242_9A14_73C63A3545B0
#ifndef bytes_bbbd2074549246cfacce7f2efcea1d5a
#define bytes_bbbd2074549246cfacce7f2efcea1d5a

#include <stdint.h>

/**
* @brief A useful little structure to refer to memory regions. Contains a pointer to the
* region, and its size.
*/
typedef struct {
	const unsigned char *data;
	size_t length;
} bytes_t;

#endif

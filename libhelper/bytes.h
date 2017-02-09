#ifndef bytes_BBBD2074549246CFACCE7F2EFCEA1D5A
#define bytes_BBBD2074549246CFACCE7F2EFCEA1D5A


#include <stdint.h>

/**
* @brief A useful little structure to refer to memory regions. Contains a pointer to the
* region, and its size.
*/
typedef struct {
	const unsigned char *data;
	size_t length;
} bytes_t;

#endif // bytes_BBBD2074549246CFACCE7F2EFCEA1D5A
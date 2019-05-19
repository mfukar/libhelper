#include "text.h"
#include <ctype.h>
#include <string.h>

bool strip (char * dst, const char * src, size_t ndst) {
    size_t nleading = 0, ntrailing = 0;

    size_t srclen = strlen (src);
    memset (dst, 0, ndst);

    for (const char *tmp = src; *tmp != 0 && isblank (*tmp); ++tmp) {
        ++nleading;
    }
    
    /* Don't bother counting trailing if the entire string is whitespace: */
    if (nleading == srclen) {
        return false;
    }

    for (const char *tmp = src + srclen - 1; *tmp != 0 && isblank (*tmp); --tmp) {
        ++ntrailing;
    }

    void * ret = memcpy (dst, src + nleading, srclen - nleading - ntrailing);

    return ret && nleading + ntrailing != 0;
}
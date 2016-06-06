/**
* @file        /libhelper/src/kmp.c
* @author      Michael Foukarakis
* @version     <+version+>
* @date        Created:     Mon Mar 03, 2014 09:15 GMT
*              Last Update: Mon Jun 06, 2016 12:12 GTBDT
*------------------------------------------------------------------------
* Description: <+description+>
*------------------------------------------------------------------------
* History:     <+history+>
* TODO:        <+missing features+>
*------------------------------------------------------------------------
* -*- coding: utf-8 -*-
*------------------------------------------------------------------------
*/
#include <string.h>
#include "kmp.h"

size_t kmp (const char *haystack, size_t hlen, const char *needle, size_t nlen, size_t *ff) {
	if (!hlen || !nlen) {
		return -1;
	}

	size_t m = 0, i = 0;

	while (m + i < hlen) {
		if (needle[i] == haystack[m + i]) {
			if (i == nlen - 1) {
				return m;
			}
			++i;
		} else {
			m += i - ff[i];

			if (ff[i] > -1) {
				i = ff[i];
			} else {
				i = 0;
			}
		}
	}

	return -1;
}

void kmp_ff (const char *needle, size_t *ff, size_t nff) {
	if (nff > 1) {
		ff[0] = -1;
		ff[1] = 0;
	} else if (nff > 0) {
		ff[0] = -1;
	}

	for (size_t pos = 2, cnd = 0; pos < nff; ) {
		if (needle[pos - 1] == needle[cnd]) {
			ff[pos++] = ++cnd;
		} else if (cnd > 0) {
			cnd = ff[cnd];
		} else {
			ff[pos++] = 0;
		}
	}
}

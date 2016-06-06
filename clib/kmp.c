/**
* @file        /Users/mfukar/src/clib/src/kmp.c
* @author      Michael Foukarakis
* @version     <+version+>
* @date        Created:     Mon Mar 03, 2014 09:15 GMT
*              Last Update: Mon Mar 03, 2014 09:18 GMT
*------------------------------------------------------------------------
* Description: <+description+>
*------------------------------------------------------------------------
* History:     <+history+>
* TODO:        <+missing features+>
*------------------------------------------------------------------------
* -*- coding: utf-8 -*-
*------------------------------------------------------------------------
*/
#include "stdafx.h"

int kmp (const char *haystack, const char *needle, int *ff) {
	int hlen = strlen (haystack);
	int nlen = strlen (needle);

	if (!hlen || !nlen) {
		return -1;
	}

	int m = 0, i = 0;

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

void kmp_ff (const char *needle, int *ff, size_t nff) {
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

/**
* @file        kmp.h
* @author      Michael Foukarakis
* @version     <+version+>
* @date        Created:     Mon Mar 03, 2014 09:11 GMT
*              Last Update: Mon Mar 03, 2014 09:15 GMT
*------------------------------------------------------------------------
* Description: <+description+>
*------------------------------------------------------------------------
* History:     <+history+>
* TODO:        <+missing features+>
*------------------------------------------------------------------------
* -*- coding: utf-8 -*-
*------------------------------------------------------------------------
*/
#ifndef kmp_37B632A1D8A24097B7E85FAB69FFEFE7__
#define kmp_37B632A1D8A24097B7E85FAB69FFEFE7__

#include <stdint.h>

/**
* @brief
*/
size_t kmp (const char *haystack, size_t hlen, const char *needle, size_t nlen, size_t *ff);

/**
*
*/
void kmp_ff (const char *needle, size_t *ff, size_t nff);

#endif // kmp_37B632A1D8A24097B7E85FAB69FFEFE7__

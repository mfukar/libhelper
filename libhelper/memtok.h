/**
* @file        /libhelper/src/memtok.h
* @author      Michael Foukarakis
* @version     <+version+>
* @date        Created:     Fri Oct 24, 2014 12:42 SAST
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
#ifndef memtok_3B849C5F87BD41179E328E606F06DA9C
#define memtok_3B849C5F87BD41179E328E606F06DA9C

#include "bytes.h"

/**
 * @brief Parse a memory buffer into a sequence of tokens.
 * 
 * The first call to memtok has a non-null first argument; subsequent calls
 * have a null first argument.
 */
bytes_t memtok (const void * restrict s, size_t length, const char * restrict delim, bytes_t * restrict save_ptr);

#endif // memtok_3B849C5F87BD41179E328E606F06DA9C

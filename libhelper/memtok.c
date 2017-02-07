/**
 * @file        /libhelper/src/memtok.c
 * @author      Michael Foukarakis
 * @version     <+version+>
 * @date        Created:     Oct 24, 2011 12:41 EEST
 *              Last Update: Wed Oct 26, 2016 11:27 CEST
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

bytes_t memtok (const void *s, size_t length, const char *delim, bytes_t *save_ptr) {
    const unsigned char *stream;
    size_t len = 0;

    if (!s) {
        stream = save_ptr->data;
    } else {
        stream = s;
        save_ptr->length = length;
    }

    const unsigned char *token = stream;

    /* Advance until either a token is found or the stream exhausted: */
    while (save_ptr->length--) {
        if (memchr (delim, *stream, strlen (delim))) {
            /*
             * Point save_ptr past the (non-existent) token.
             * This is OK as long as we do not dereference one-past whatever buffer we are
             * passed, and we never will if the caller does not mess with save_ptr.
             */
            save_ptr->data = stream + 1;
            return (bytes_t) { .data = token, .length = len };
        }

        ++len;
        ++stream;
    }

    /* State : done. */
    *save_ptr = (bytes_t) { .data = NULL, .length = 0 };

    /* Stream exhausted but no delimiters terminate it. */
    return (bytes_t) { .data = token, .length = len };
}

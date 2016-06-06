/**
* @file        /libhelper/src/preprocessor.h
* @author      Michael Foukarakis
* @version     0.1
* @date        Created:     Thu Dec 19, 2013 11:48 EET
*              Last Update: Mon Jun 06, 2016 12:13 GTBDT
*------------------------------------------------------------------------
* Description: <+description+>
*------------------------------------------------------------------------
* History:     <+history+>
* TODO:        <+missing features+>
*------------------------------------------------------------------------
* -*- coding: utf-8 -*-
*------------------------------------------------------------------------
*/
#ifndef preprocessor_8C9E86E0AFB24D149A77C00CFB7ADFB2
#define preprocessor_8C9E86E0AFB24D149A77C00CFB7ADFB2

/**
* A preprocessor macro tying resource management to the scope of a variable for FILE
* objects.
*
* @param varname
* @param fname
* @param options
* @param block
* @param error
*/
#define WITH_FILE(varname, fname, options, block, error) do {\
    FILE * const varname = fopen(fname, options); \
    if (varname) { \
        block; \
        fclose(varname); \
    } else { \
        error; \
    } \
} while (0)


/**
* A preprocessor macro tying resource management to the scope of a variable for heap
* memory obtained with malloc(3).
*
* @param varname
* @param type
* @param count
* @param block
* @param error
*/
#define WITH_MALLOC(varname, type, count, block, error) do { \
    type * const varname = malloc( (count) * sizeof (type)); \
    if (varname) { \
        block; \
        free(varname); \
    } else { \
        error; \
    } \
} while(0)

#endif // preprocessor_8C9E86E0AFB24D149A77C00CFB7ADFB2

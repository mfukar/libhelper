// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <assert.h>

#include "aho-corasick.h"
#include "assertions.h"
#include "bytes.h"
#include "dlist.h"
#include "kmp.h"
#include "memtok.h"
#include "overflow.h"
#include "preprocessor.h"
#include "queue.h"
#include "slist.h"

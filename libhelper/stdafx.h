#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */

#ifdef _MSC_VER
#define restrict __restrict
#endif

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

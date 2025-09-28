/* $Id$ */
#ifndef __MILSKO_MACHDEP_H__
#define __MILSKO_MACHDEP_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(_MILSKO) && defined(_WIN32)
#define MILSKODECL extern __declsped(dllexport)
#elif defined(_WIN32)
#define MILSKODECL extern __declsped(dllimport)
#else
#define MILSKODECL extern
#endif

#endif

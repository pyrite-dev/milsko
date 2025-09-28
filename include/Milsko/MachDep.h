/* $Id$ */
#ifndef __MILSKO_MACHDEP_H__
#define __MILSKO_MACHDEP_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#else
#include <unistd.h>
#endif

#if defined(_MILSKO) && defined(_WIN32)
#define MILSKODECL extern __declsped(dllexport)
#elif defined(_WIN32)
#define MILSKODECL extern __declsped(dllimport)
#else
#define MILSKODECL extern
#endif

#endif

/* $Id$ */
/*!
 * %file Mw/MachDep.h
 * %brief Machine dependent headers and macros
 */
#ifndef __MW_MACHDEP_H__
#define __MW_MACHDEP_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <sys/types.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#include <dlfcn.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif

#if defined(_MILSKO) && defined(_WIN32)
#define MWDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define MWDECL extern __declspec(dllimport)
#else
#define MWDECL extern
#endif

#endif

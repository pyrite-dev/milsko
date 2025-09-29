/* $Id$ */
#ifndef __MW_MACHDEP_H__
#define __MW_MACHDEP_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined(_MW) && defined(_WIN32)
#define MWDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define MWDECL extern __declspec(dllimport)
#else
#define MWDECL extern
#endif

#endif

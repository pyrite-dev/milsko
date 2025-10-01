/* $Id$ */
/*!
 * %file Mw/MachDep.h
 * %param Machine dependent headers and macros
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
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#if defined(_MILSKO) && defined(_WIN32)
#define MWDECL extern __declspec(dllexport)
#elif defined(_WIN32)
#define MWDECL extern __declspec(dllimport)
#else
#define MWDECL extern
#endif

#endif

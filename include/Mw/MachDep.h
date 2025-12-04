/*!
 * @file Mw/MachDep.h
 * @brief Machine dependent headers and macros
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
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>

#ifdef _MILSKO
#include <windows.h>
#ifndef GWLP_USERDATA
#define GWLP_USERDATA GWL_USERDATA
#define GWLP_WNDPROC GWL_WNDPROC
#define GCLP_HICON GCL_HICON
#define GCLP_HCURSOR GCL_HCURSOR
#define SetWindowLongPtr SetWindowLong
#define GetWindowLongPtr GetWindowLong
#define SetClassLongPtr SetClassLong
#define GetClassLongPtr GetClassLong
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020a
#define GET_WHEEL_DELTA_WPARAM(x) ((short)HIWORD(x))
#endif
#endif

#else
#include <unistd.h>
#include <pwd.h>
#include <dlfcn.h>
#include <signal.h>
#include <dirent.h>
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

#define MwInline static __inline

#endif

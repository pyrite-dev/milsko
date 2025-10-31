/* $Id$ */
/*!
 * @file Mw/BaseTypes.h
 * @brief Base type definitions
 */
#ifndef __MW_BASETYPES_H__
#define __MW_BASETYPES_H__

typedef struct _MwPoint	    MwPoint;
typedef struct _MwRect	    MwRect;
typedef struct _MwSizeHints MwSizeHints;
typedef struct _MwFont	    MwFont;
typedef struct _MwCursor    MwCursor;

typedef unsigned char MwBool;
#define MwTRUE ((MwBool)1)
#define MwFALSE ((MwBool)0)

#if __STDC_VERSION__ >= 199901L || __GNUC__ > 2
typedef long long MwI64;
typedef unsigned long long MwU64;
#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__)
typedef __int64 MwI64;
typedef unsigned __int64 MwU64;
#else
/* out of hope */
typedef long MwI64;
typedef unsigned long MwU64;
#endif

typedef MwI64 MwOffset;

struct _MwPoint {
	int x;
	int y;
};

struct _MwRect {
	int	     x;
	int	     y;
	unsigned int width;
	unsigned int height;
};

struct _MwSizeHints {
	int min_width;
	int min_height;
	int max_width;
	int max_height;
};

#define MwCursorDataHeight 16
struct _MwCursor {
	int	     width;
	int	     height;
	int	     x;
	int	     y;
	unsigned int data[MwCursorDataHeight];
};

struct _MwFont {
	int	      left;
	int	      top;
	unsigned char data[16];
};

#endif

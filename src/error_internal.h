/* $Id$ */

#ifndef __MW_ERROR_INTERNAL_H__
#define __MW_ERROR_INTERNAL_H__

// This is not to be documented or exposed publically.
// This contains an internal function for setting the error,
// something the user shouldn't want to do and thus we don't want them to.
// (hence also, its placement in the src directory)

#include <Mw/Error.h>

void setLastError(const char* fmt, ...);

#endif

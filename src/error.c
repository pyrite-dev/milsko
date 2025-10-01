/* $Id$ */
#include <Mw/Milsko.h>
#include "error_internal.h"

#define MAX_ERROR_LEN 512

// buffer for holding the error. +1 to ensure there's always a null terminator.
char error[MAX_ERROR_LEN + 1] = {0};

const char* MwGetLastError() {
	return error;
}

void setLastError(const char* fmt, ...) {
	va_list va;
	char	out[MAX_ERROR_LEN];
	memset(out, 0, MAX_ERROR_LEN);

	va_start(va, fmt);
	vsnprintf(out, MAX_ERROR_LEN, fmt, va);
	va_end(va);

	memcpy(error, out, MAX_ERROR_LEN);
}

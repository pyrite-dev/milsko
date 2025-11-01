/* $Id$ */
#include <Mw/BaseTypes.h>
#include "math_internal.h"

static char endian = 0;

char nbsd_endian(void) {
	unsigned short n = 1;
	if(endian != 0) return endian;

	if(1 == *(unsigned char*)&n) {
		endian = 'L';
	} else {
		endian = 'B';
	}
	return endian;
}

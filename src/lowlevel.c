/* $Id$ */
#include <Mw/Mw.h>

void MwLLCreateCommon(MwLL handle) {
	handle->handler = malloc(sizeof(*handle->handler));
	memset(handle->handler, 0, sizeof(*handle->handler));
}

void MwLLDestroyCommon(MwLL handle) {
	free(handle->handler);
}

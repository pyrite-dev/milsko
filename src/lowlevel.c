/* $Id$ */
#include <Milsko/Milsko.h>

void MilskoLLCreateCommon(MilskoLL handle) {
	handle->handler = malloc(sizeof(*handle->handler));
	memset(handle->handler, 0, sizeof(*handle->handler));
}

void MilskoLLDestroyCommon(MilskoLL handle) {
	free(handle->handler);
}

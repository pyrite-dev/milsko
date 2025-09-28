/* $Id$ */
#include <Milsko/Milsko.h>

void MilskoLLCreateCommon(MilskoLL handle) {
	handle->callback = malloc(sizeof(*handle->callback));
	memset(handle->callback, 0, sizeof(*handle->callback));
}

void MilskoLLDestroyCommon(MilskoLL handle) {
	free(handle->callback);
}

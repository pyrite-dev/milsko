/* $Id$ */
#include <Milsko/Milsko.h>

#include "stb_ds.h"

HMILSKO MilskoCreateWidget(MilskoClass class, HMILSKO parent, int x, int y, unsigned int width, unsigned int height) {
	HMILSKO h = malloc(sizeof(*h));

	h->parent   = parent;
	h->children = NULL;
	h->lowlevel = MilskoLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height);
	h->class    = class;

	if(parent != NULL) arrput(parent->children, h);

	return h;
}

void MilskoDestroyWidget(HMILSKO handle) {
	int i;

	if(handle->children != NULL) {
		for(i = 0; i < arrlen(handle->children); i++) {
			if(handle->children[i] == handle) {
				MilskoDestroyWidget(handle->children[i]);
				break;
			}
		}
		arrfree(handle->children);
	}

	if(handle->parent != NULL) {
		for(i = 0; i < arrlen(handle->parent->children); i++) {
			if(handle->parent->children[i] == handle) {
				arrdel(handle->parent->children, i);
				break;
			}
		}
	}
	MilskoLLDestroy(handle->lowlevel);
	free(handle);
}

MILSKODECL void MilskoStep(HMILSKO handle) {
	MilskoLLNextEvent(handle->lowlevel);
}

MILSKODECL int MilskoPending(HMILSKO handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) {
		if(MilskoPending(handle->children[i])) return 1;
	}
	return MilskoLLPending(handle->lowlevel);
}

MILSKODECL void MilskoLoop(HMILSKO handle) {
	while(1) {
		MilskoStep(handle);
		MilskoLLSleep(10);
	}
}

void MilskoSetInteger(HMILSKO handle, const char* key, int n) {
	int xy = 0;
	int wh = 0;
	if((xy = (strcmp(key, MilskoNx) == 0 || strcmp(key, MilskoNy) == 0)) || (wh = (strcmp(key, MilskoNwidth) == 0 || strcmp(key, MilskoNheight) == 0))) {
		int	     x, y;
		unsigned int w, h;

		MilskoLLGetXYWH(handle->lowlevel, &x, &y, &w, &h);
		if(strcmp(key, MilskoNx) == 0) x = n;
		if(strcmp(key, MilskoNy) == 0) y = n;
		if(strcmp(key, MilskoNwidth) == 0) w = n;
		if(strcmp(key, MilskoNheight) == 0) h = n;
		if(xy) MilskoLLSetXY(handle->lowlevel, x, y);
		if(wh) MilskoLLSetWH(handle->lowlevel, w, h);
	}
}

void MilskoApply(HMILSKO handle, ...) {
	va_list va;
	char*	key;

	va_start(va, handle);
	while((key = va_arg(va, char*)) != NULL) {
		if(key[0] == 'I') {
			int n = va_arg(va, int);
			MilskoSetInteger(handle, key, n);
		}
	}
	va_end(va);
}

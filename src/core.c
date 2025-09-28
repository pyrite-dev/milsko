/* $Id$ */
#include <Milsko/Milsko.h>

#include "stb_ds.h"

static void lldrawhandler(MilskoLL handle) {
	MilskoWidget h = (MilskoWidget)handle->user;
	MilskoDispatch(h, draw);
}

static void lluphandler(MilskoLL handle) {
	MilskoWidget h = (MilskoWidget)handle->user;
	h->pressed     = 0;

	MilskoDispatch(h, click);
}

static void lldownhandler(MilskoLL handle) {
	MilskoWidget h = (MilskoWidget)handle->user;
	h->pressed     = 1;
}

MilskoWidget MilskoCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height) {
	MilskoWidget h = malloc(sizeof(*h));

	h->name = malloc(strlen(name) + 1);
	strcpy(h->name, name);

	h->parent   = parent;
	h->children = NULL;
	h->lowlevel = MilskoLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height);
	h->class    = class;
	h->pressed  = 0;

	h->lowlevel->user	   = h;
	h->lowlevel->handler->draw = lldrawhandler;
	h->lowlevel->handler->up   = lluphandler;
	h->lowlevel->handler->down = lldownhandler;

	if(parent != NULL) arrput(parent->children, h);

	sh_new_strdup(h->text);
	sh_new_strdup(h->integer);

	shdefault(h->integer, -1);
	shdefault(h->text, NULL);
	shdefault(h->handler, NULL);

	MilskoDispatch(h, create);

	return h;
}

MilskoWidget MilskoVaCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height, ...) {
	MilskoWidget h;
	va_list	     va;

	va_start(va, height);
	h = MilskoVaListCreateWidget(class, name, parent, x, y, width, height, va);
	va_end(va);

	return h;
}

MilskoWidget MilskoVaListCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va) {
	MilskoWidget h;

	h = MilskoCreateWidget(class, name, parent, x, y, width, height);
	MilskoVaListApply(h, va);

	return h;
}

void MilskoDestroyWidget(MilskoWidget handle) {
	int i;

	MilskoDispatch(handle, destroy);

	free(handle->name);

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

	shfree(handle->integer);

	for(i = 0; i < shlen(handle->text); i++) {
		free(handle->text[i].value);
		handle->text[i].value = NULL;
	}
	shfree(handle->text);

	free(handle);
}

void MilskoStep(MilskoWidget handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) MilskoStep(handle->children[i]);
	MilskoLLNextEvent(handle->lowlevel);
}

int MilskoPending(MilskoWidget handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) {
		if(MilskoPending(handle->children[i])) return 1;
	}
	return MilskoLLPending(handle->lowlevel);
}

void MilskoLoop(MilskoWidget handle) {
	while(1) {
		MilskoStep(handle);
		MilskoLLSleep(5);
	}
}

void MilskoSetInteger(MilskoWidget handle, const char* key, int n) {
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
	} else {
		shput(handle->integer, key, n);
	}
}

void MilskoSetText(MilskoWidget handle, const char* key, const char* value) {
	if(strcmp(key, MilskoNtitle) == 0) {
		MilskoLLSetTitle(handle->lowlevel, value);
	} else {
		char* v = malloc(strlen(value) + 1);
		strcpy(v, value);

		if(shgeti(handle->text, key) != -1) free(shget(handle->text, key));

		shput(handle->text, key, v);
	}
}

void MilskoSetHandler(MilskoWidget handle, const char* key, MilskoHandler value) {
	shput(handle->handler, key, value);
}

int MilskoGetInteger(MilskoWidget handle, const char* key) {
	if(strcmp(key, MilskoNx) == 0 || strcmp(key, MilskoNy) == 0 || strcmp(key, MilskoNwidth) == 0 || strcmp(key, MilskoNheight) == 0) {
		int	     x, y;
		unsigned int w, h;

		MilskoLLGetXYWH(handle->lowlevel, &x, &y, &w, &h);

		if(strcmp(key, MilskoNx) == 0) return x;
		if(strcmp(key, MilskoNy) == 0) return y;
		if(strcmp(key, MilskoNwidth) == 0) return w;
		if(strcmp(key, MilskoNheight) == 0) return h;
		return -1;
	} else {
		return shget(handle->integer, key);
	}
}

const char* MilskoGetText(MilskoWidget handle, const char* key) {
	return shget(handle->text, key);
}

MilskoHandler MilskoGetHandler(MilskoWidget handle, const char* key) {
	return shget(handle->handler, key);
}

void MilskoVaApply(MilskoWidget handle, ...) {
	va_list va;

	va_start(va, handle);
	MilskoVaListApply(handle, va);
	va_end(va);
}

void MilskoVaListApply(MilskoWidget handle, va_list va) {
	char* key;

	while((key = va_arg(va, char*)) != NULL) {
		if(key[0] == 'I') {
			int n = va_arg(va, int);
			MilskoSetInteger(handle, key, n);
		} else if(key[0] == 'S') {
			char* t = va_arg(va, char*);
			MilskoSetText(handle, key, t);
		} else if(key[0] == 'C') {
			MilskoHandler h = va_arg(va, MilskoHandler);
			MilskoSetHandler(handle, key, h);
		}
	}
}

void MilskoSetDefault(MilskoWidget handle) {
	MilskoSetText(handle, MilskoNbackground, MilskoDefaultBackground);
}

void MilskoDispatchHandler(MilskoWidget handle, const char* key) {
	MilskoHandler handler = MilskoGetHandler(handle, key);
	if(handler != NULL) handler(handle);
}

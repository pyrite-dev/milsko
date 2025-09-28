/* $Id$ */
#include <Milsko/Milsko.h>

#include "stb_ds.h"

#define Dispatch(x, y) \
	if(x->class != NULL && x->class->y != NULL) x->class->y(x)

static void llhandler(MilskoLL handle) {
	MilskoWidget h = (MilskoWidget)handle->user;
	Dispatch(h, draw);
}

MilskoWidget MilskoCreateWidget(MilskoClass class, const char* name, MilskoWidget parent, int x, int y, unsigned int width, unsigned int height) {
	MilskoWidget h = malloc(sizeof(*h));

	h->name = malloc(strlen(name) + 1);
	strcpy(h->name, name);

	h->parent   = parent;
	h->children = NULL;
	h->lowlevel = MilskoLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height);
	h->class    = class;

	h->lowlevel->user = h;
	h->lowlevel->draw = llhandler;

	if(parent != NULL) arrput(parent->children, h);

	sh_new_strdup(h->text);
	sh_new_strdup(h->integer);

	shdefault(h->text, NULL);
	shdefault(h->integer, -1);

	Dispatch(h, create);

	return h;
}

void MilskoDestroyWidget(MilskoWidget handle) {
	int i;

	Dispatch(handle, destroy);

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

		shput(handle->text, key, v);
	}
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

void MilskoApply(MilskoWidget handle, ...) {
	va_list va;
	char*	key;

	va_start(va, handle);
	while((key = va_arg(va, char*)) != NULL) {
		if(key[0] == 'I') {
			int n = va_arg(va, int);
			MilskoSetInteger(handle, key, n);
		} else if(key[0] == 'S') {
			char* t = va_arg(va, char*);
			MilskoSetText(handle, key, t);
		}
	}
	va_end(va);
}

static int hex(const char* txt, int len) {
	int i;
	int r = 0;
	for(i = 0; i < len; i++) {
		char c = txt[i];
		int  n = 0;
		if('a' <= c && c <= 'f') {
			n = c - 'a' + 10;
		} else if('A' <= c && c <= 'F') {
			n = c - 'A' + 10;
		} else if('0' <= c && c <= '9') {
			n = c - '0';
		}
		r = r << 4;
		r |= n;
	}
	return r;
}

MilskoLLColor MilskoParseColor(MilskoWidget handle, const char* text) {
	int r = 0;
	int g = 0;
	int b = 0;

	if(text[0] == '#' && strlen(text) == 4) {
		r = hex(text + 1, 1);
		g = hex(text + 2, 1);
		b = hex(text + 3, 1);

		r |= r << 4;
		g |= g << 4;
		b |= b << 4;
	} else if(text[0] == '#' && strlen(text) == 7) {
		r = hex(text + 1, 2);
		g = hex(text + 3, 2);
		b = hex(text + 5, 2);
	}

	return MilskoLLAllocColor(handle->lowlevel, r, g, b);
}

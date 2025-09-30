/* $Id$ */
#include <Mw/Milsko.h>

#include "stb_ds.h"

static void lldrawhandler(MwLL handle) {
	MwWidget h = (MwWidget)handle->user;
	MwDispatch(h, draw);
}

static void lluphandler(MwLL handle) {
	MwWidget h = (MwWidget)handle->user;
	h->pressed = 0;

	MwDispatch(h, click);
}

static void lldownhandler(MwLL handle) {
	MwWidget h = (MwWidget)handle->user;
	h->pressed = 1;
}

static void llresizehandler(MwLL handle) {
	MwWidget h = (MwWidget)handle->user;

	MwDispatchUserHandler(h, MwNresizeHandler, NULL);
}

static void llclosehandler(MwLL handle) {
	MwWidget h = (MwWidget)handle->user;

	h->close = 1;
}

MwWidget MwCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height) {
	MwWidget h = malloc(sizeof(*h));

	h->name = malloc(strlen(name) + 1);
	strcpy(h->name, name);

	h->parent	= parent;
	h->children	= NULL;
	h->lowlevel	= MwLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height);
	h->widget_class = widget_class;
	h->pressed	= 0;
	h->close	= 0;

	h->lowlevel->user	     = h;
	h->lowlevel->handler->draw   = lldrawhandler;
	h->lowlevel->handler->up     = lluphandler;
	h->lowlevel->handler->down   = lldownhandler;
	h->lowlevel->handler->resize = llresizehandler;
	h->lowlevel->handler->close  = llclosehandler;

	if(parent != NULL) arrput(parent->children, h);

	sh_new_strdup(h->text);
	sh_new_strdup(h->integer);
	sh_new_strdup(h->handler);
	sh_new_strdup(h->data);

	shdefault(h->integer, -1);
	shdefault(h->text, NULL);
	shdefault(h->handler, NULL);
	shdefault(h->data, NULL);

	MwDispatch(h, create);

	return h;
}

MwWidget MwVaCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, ...) {
	MwWidget h;
	va_list	 va;

	va_start(va, height);
	h = MwVaListCreateWidget(widget_class, name, parent, x, y, width, height, va);
	va_end(va);

	return h;
}

MwWidget MwVaListCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height, va_list va) {
	MwWidget h;

	h = MwCreateWidget(widget_class, name, parent, x, y, width, height);
	MwVaListApply(h, va);

	return h;
}

void MwDestroyWidget(MwWidget handle) {
	int i;

	MwDispatch(handle, destroy);

	free(handle->name);

	if(handle->children != NULL) {
		for(i = 0; i < arrlen(handle->children); i++) {
			if(handle->children[i] == handle) {
				MwDestroyWidget(handle->children[i]);
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
	MwLLDestroy(handle->lowlevel);

	shfree(handle->integer);

	for(i = 0; i < shlen(handle->text); i++) {
		free(handle->text[i].value);
		handle->text[i].value = NULL;
	}
	shfree(handle->text);
	shfree(handle->handler);
	shfree(handle->data);

	free(handle);
}

void MwStep(MwWidget handle) {
	int i;
	if(setjmp(handle->before_step)) return;
	for(i = 0; i < arrlen(handle->children); i++) MwStep(handle->children[i]);
	MwLLNextEvent(handle->lowlevel);
}

int MwPending(MwWidget handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) {
		if(MwPending(handle->children[i])) return 1;
	}
	return MwLLPending(handle->lowlevel);
}

void MwLoop(MwWidget handle) {
	while(!handle->close) {
		MwStep(handle);

		MwDispatchUserHandler(handle, MwNtickHandler, NULL);
		MwLLSleep(MwWaitMS);
	}
}

void MwSetInteger(MwWidget handle, const char* key, int n) {
	int xy = 0;
	int wh = 0;
	if((xy = (strcmp(key, MwNx) == 0 || strcmp(key, MwNy) == 0)) || (wh = (strcmp(key, MwNwidth) == 0 || strcmp(key, MwNheight) == 0))) {
		int	     x, y;
		unsigned int w, h;

		MwLLGetXYWH(handle->lowlevel, &x, &y, &w, &h);
		if(strcmp(key, MwNx) == 0) x = n;
		if(strcmp(key, MwNy) == 0) y = n;
		if(strcmp(key, MwNwidth) == 0) w = n;
		if(strcmp(key, MwNheight) == 0) h = n;
		if(xy) MwLLSetXY(handle->lowlevel, x, y);
		if(wh) MwLLSetWH(handle->lowlevel, w, h);
	} else {
		shput(handle->integer, key, n);
	}
}

void MwSetText(MwWidget handle, const char* key, const char* value) {
	if(strcmp(key, MwNtitle) == 0) {
		MwLLSetTitle(handle->lowlevel, value);
	} else {
		char* v = malloc(strlen(value) + 1);
		strcpy(v, value);

		if(shgeti(handle->text, key) != -1) free(shget(handle->text, key));

		shput(handle->text, key, v);
	}
}

void MwSetVoid(MwWidget handle, const char* key, void* value) {
	shput(handle->data, key, value);
}

int MwGetInteger(MwWidget handle, const char* key) {
	if(strcmp(key, MwNx) == 0 || strcmp(key, MwNy) == 0 || strcmp(key, MwNwidth) == 0 || strcmp(key, MwNheight) == 0) {
		int	     x, y;
		unsigned int w, h;

		MwLLGetXYWH(handle->lowlevel, &x, &y, &w, &h);

		if(strcmp(key, MwNx) == 0) return x;
		if(strcmp(key, MwNy) == 0) return y;
		if(strcmp(key, MwNwidth) == 0) return w;
		if(strcmp(key, MwNheight) == 0) return h;
		return -1;
	} else {
		return shget(handle->integer, key);
	}
}

const char* MwGetText(MwWidget handle, const char* key) {
	return shget(handle->text, key);
}

void* MwGetVoid(MwWidget handle, const char* key) {
	return shget(handle->data, key);
}

void MwVaApply(MwWidget handle, ...) {
	va_list va;

	va_start(va, handle);
	MwVaListApply(handle, va);
	va_end(va);
}

void MwVaListApply(MwWidget handle, va_list va) {
	char* key;

	while((key = va_arg(va, char*)) != NULL) {
		if(key[0] == 'I') {
			int n = va_arg(va, int);
			MwSetInteger(handle, key, n);
		} else if(key[0] == 'S') {
			char* t = va_arg(va, char*);
			MwSetText(handle, key, t);
		} else if(key[0] == 'C') {
			MwUserHandler h = va_arg(va, MwUserHandler);
			int	      ind;

			shput(handle->handler, key, h);
			ind			       = shgeti(handle->handler, key);
			handle->handler[ind].user_data = NULL;
		} else if(key[0] == 'V') {
			void* v = va_arg(va, void*);
			MwSetVoid(handle, key, v);
		}
	}
}

void MwSetDefault(MwWidget handle) {
	MwSetText(handle, MwNbackground, MwDefaultBackground);
	MwSetText(handle, MwNforeground, MwDefaultForeground);
}

void MwDispatchUserHandler(MwWidget handle, const char* key, void* handler_data) {
	int ind = shgeti(handle->handler, key);
	if(ind == -1) return;

	handle->handler[ind].value(handle, handle->handler[ind].user_data, handler_data);
}

void MwAddUserHandler(MwWidget handle, const char* key, MwUserHandler handler, void* user_data) {
	int ind;

	shput(handle->handler, key, handler);
	ind			       = shgeti(handle->handler, key);
	handle->handler[ind].user_data = user_data;
}

static MwErrorHandler error_handler   = NULL;
static void*	      error_user_data = NULL;

void MwSetErrorHandler(MwErrorHandler handler, void* user_data) {
	error_handler	= handler;
	error_user_data = user_data;
}

void MwDispatchError(int code, const char* message) {
	if(error_handler != NULL) {
		error_handler(code, message, error_user_data);
	} else {
#ifdef _WIN32
		char buffer[1024];
		sprintf(buffer, "Error: %s\r\nCode : %d\r\n\r\nMilsko is exiting.", message, code);
		MessageBox(NULL, buffer, "Error", MB_ICONERROR | MB_OK);
#else
		fprintf(stderr, "Error: %s\nCode : %d\n\nMilsko is exiting.", message, code);
#endif
		exit(1);
	}
}

void MwGetBeforeStep(MwWidget handle, jmp_buf* jmpbuf) {
	memcpy(jmpbuf, &handle->before_step, sizeof(*jmpbuf));
}

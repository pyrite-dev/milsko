/* $Id$ */
#include <Mw/Milsko.h>

#include "external/stb_ds.h"

static void lldrawhandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	(void)data;

	MwDispatch(h, draw);
}

static void lluphandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;
	MwPoint* p = data;

	(void)data;

	h->pressed	 = 0;
	h->mouse_point.x = p->x;
	h->mouse_point.y = p->y;

	MwDispatch(h, click);
	MwDispatch(h, mouse_up);
	MwDispatchUserHandler(h, MwNmouseUpHandler, data);
}

static void lldownhandler(MwLL handle, void* data) {
	MwWidget h	 = (MwWidget)handle->user;
	MwPoint* p	 = data;
	h->pressed	 = 1;
	h->mouse_point.x = p->x;
	h->mouse_point.y = p->y;

	MwDispatch(h, mouse_down);
	MwDispatchUserHandler(h, MwNmouseDownHandler, data);
}

static void llresizehandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;
	int	 i;

	(void)data;

	MwDispatchUserHandler(h, MwNresizeHandler, NULL);
	for(i = 0; i < arrlen(h->children); i++) {
		MwDispatch(h->children[i], parent_resize);
	}
}

static void llclosehandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	(void)data;

	h->close = 1;
}

static void llmovehandler(MwLL handle, void* data) {
	MwWidget h	 = (MwWidget)handle->user;
	MwPoint* p	 = data;
	h->mouse_point.x = p->x;
	h->mouse_point.y = p->y;

	MwDispatch(h, mouse_move);
}

static void llkeyhandler(MwLL handle, void* data) {
	MwWidget h   = (MwWidget)handle->user;
	int	 key = *(int*)data;

	MwDispatch3(h, key, key);
}

MwWidget MwCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height) {
	MwWidget h = malloc(sizeof(*h));

	h->name = malloc(strlen(name) + 1);
	strcpy(h->name, name);

	h->parent   = parent;
	h->children = NULL;
	if((h->lowlevel = MwLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height)) == NULL) {
		free(h->name);
		free(h);
		return NULL;
	}
	h->widget_class	 = widget_class;
	h->pressed	 = 0;
	h->close	 = 0;
	h->destroy_queue = NULL;
	h->prop_event	 = 1;

	h->lowlevel->user	     = h;
	h->lowlevel->handler->draw   = lldrawhandler;
	h->lowlevel->handler->up     = lluphandler;
	h->lowlevel->handler->down   = lldownhandler;
	h->lowlevel->handler->resize = llresizehandler;
	h->lowlevel->handler->close  = llclosehandler;
	h->lowlevel->handler->move   = llmovehandler;
	h->lowlevel->handler->key    = llkeyhandler;

	if(parent != NULL) arrput(parent->children, h);

	sh_new_strdup(h->text);
	sh_new_strdup(h->integer);
	sh_new_strdup(h->handler);
	sh_new_strdup(h->data);

	shdefault(h->integer, -1);
	shdefault(h->text, NULL);
	shdefault(h->handler, NULL);
	shdefault(h->data, NULL);

	if(MwDispatch2(h, create) != 0) {
		h->widget_class = NULL;
		MwDestroyWidget(h);
		return NULL;
	}

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

static void MwFreeWidget(MwWidget handle) {
	int i;

	MwDispatch(handle, destroy);

	for(i = 0; i < arrlen(handle->children); i++) {
		MwFreeWidget(handle->children[i]);
	}

	free(handle->name);

	MwLLDestroy(handle->lowlevel);

	shfree(handle->integer);

	for(i = 0; i < shlen(handle->text); i++) {
		free(handle->text[i].value);
		handle->text[i].value = NULL;
	}
	shfree(handle->text);
	shfree(handle->handler);
	shfree(handle->data);

	arrfree(handle->destroy_queue);

	free(handle);
}

void MwDestroyWidget(MwWidget handle) {
	if(handle->parent != NULL) {
		arrput(handle->parent->destroy_queue, handle);
	}
}

void MwStep(MwWidget handle) {
	int i, j;
	if(setjmp(handle->before_step)) return;
	for(i = 0; i < arrlen(handle->children); i++) MwStep(handle->children[i]);

	handle->prop_event = 0;
	MwLLNextEvent(handle->lowlevel);
	handle->prop_event = 1;

	for(i = 0; i < arrlen(handle->destroy_queue); i++) {
		MwWidget w = handle->destroy_queue[i];

		MwFreeWidget(w);
	}
	for(i = 0; i < arrlen(handle->destroy_queue); i++) {
		for(j = 0; j < arrlen(handle->children); j++) {
			if(handle->children[j] == handle->destroy_queue[i]) {
				arrdel(handle->children, j);
				break;
			}
		}
	}
	arrfree(handle->destroy_queue);
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
	if(handle->prop_event) MwDispatch3(handle, prop_change, key);
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
	if(handle->prop_event) MwDispatch3(handle, prop_change, key);
	if(strcmp(key, MwNbackground) == 0 || strcmp(key, MwNforeground) == 0) {
		MwForceRender(handle);
	}
}

void MwSetVoid(MwWidget handle, const char* key, void* value) {
	if(strcmp(key, MwNiconPixmap) == 0) {
		MwLLSetIcon(handle->lowlevel, value);
	} else if(strcmp(key, MwNsizeHints) == 0) {
		MwSizeHints* sz = value;
		MwLLSetSizeHints(handle->lowlevel, sz->min_width, sz->min_height, sz->max_width, sz->max_height);
	} else {
		shput(handle->data, key, value);
	}
	if(handle->prop_event) MwDispatch3(handle, prop_change, key);
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
	MwLLSetCursor(handle->lowlevel, &MwCursorDefault, &MwCursorDefaultMask);

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

void MwForceRender(MwWidget handle) {
	MwLLForceRender(handle->lowlevel);
}

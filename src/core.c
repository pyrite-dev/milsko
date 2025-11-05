/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

static void lldrawhandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	(void)data;

	h->bgcolor = NULL;
	MwDispatch(h, draw);
	if(h->draw_inject != NULL) h->draw_inject(h);
}

static void lluphandler(MwLL handle, void* data) {
	MwWidget   h = (MwWidget)handle->user;
	MwLLMouse* p = data;

	if(p->button == MwLLMouseLeft) h->pressed = 0;
	h->mouse_point.x = p->point.x;
	h->mouse_point.y = p->point.y;

	if(p->button == MwLLMouseLeft) MwDispatch(h, click);
	MwDispatch3(h, mouse_up, data);
	MwDispatchUserHandler(h, MwNmouseUpHandler, data);
}

static void lldownhandler(MwLL handle, void* data) {
	MwWidget   h = (MwWidget)handle->user;
	MwLLMouse* p = data;
	if(p->button == MwLLMouseLeft) h->pressed = 1;
	h->mouse_point.x = p->point.x;
	h->mouse_point.y = p->point.y;

	MwDispatch3(h, mouse_down, data);
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
	int	 n;

	(void)data;

	if((n = MwGetInteger(h, MwNmain)) != MwDEFAULT && n) {
		while(h != NULL) {
			h->close = 1;
			MwDispatchUserHandler(h, MwNcloseHandler, NULL);
			h = h->parent;
		}
	} else {
		h->close = 1;
		MwDispatchUserHandler(h, MwNcloseHandler, NULL);
	}
}

static void llmovehandler(MwLL handle, void* data) {
	MwWidget h	 = (MwWidget)handle->user;
	MwPoint* p	 = data;
	h->mouse_point.x = p->x;
	h->mouse_point.y = p->y;

	MwDispatch(h, mouse_move);
	MwDispatchUserHandler(h, MwNmouseMoveHandler, data);
}

static void llkeyhandler(MwLL handle, void* data) {
	MwWidget h   = (MwWidget)handle->user;
	int	 key = *(int*)data;

	MwDispatch3(h, key, key);
	MwDispatchUserHandler(h, MwNkeyHandler, data);
}

static void llkeyrelhandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	MwDispatchUserHandler(h, MwNkeyReleaseHandler, data);
}

static void llfocusinhandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	MwDispatchUserHandler(h, MwNfocusInHandler, data);
}

static void llfocusouthandler(MwLL handle, void* data) {
	MwWidget h = (MwWidget)handle->user;

	MwDispatchUserHandler(h, MwNfocusOutHandler, data);
}

MwWidget MwCreateWidget(MwClass widget_class, const char* name, MwWidget parent, int x, int y, unsigned int width, unsigned int height) {
	MwWidget h = malloc(sizeof(*h));

	h->name = MwStringDupliacte(name);

	h->parent   = parent;
	h->children = NULL;
	if(widget_class != NULL) {
		if((h->lowlevel = MwLLCreate(parent == NULL ? NULL : parent->lowlevel, x, y, width, height)) == NULL) {
			free(h->name);
			free(h);
			return NULL;
		}
	} else {
		h->lowlevel = NULL;
	}
	h->widget_class	 = widget_class;
	h->pressed	 = 0;
	h->close	 = 0;
	h->destroy_queue = NULL;
	h->prop_event	 = 1;
	h->draw_inject	 = NULL;
	h->tick_list	 = NULL;
	h->destroyed	 = 0;
	h->dark_theme	 = 0;
	h->bgcolor	 = NULL;

	if(parent == NULL) arrput(h->tick_list, h);

	if(h->lowlevel != NULL) {
		h->lowlevel->user		   = h;
		h->lowlevel->handler->draw	   = lldrawhandler;
		h->lowlevel->handler->up	   = lluphandler;
		h->lowlevel->handler->down	   = lldownhandler;
		h->lowlevel->handler->resize	   = llresizehandler;
		h->lowlevel->handler->close	   = llclosehandler;
		h->lowlevel->handler->move	   = llmovehandler;
		h->lowlevel->handler->key	   = llkeyhandler;
		h->lowlevel->handler->key_released = llkeyrelhandler;
		h->lowlevel->handler->focus_in	   = llfocusinhandler;
		h->lowlevel->handler->focus_out	   = llfocusouthandler;
	}

	if(parent != NULL) arrput(parent->children, h);

	sh_new_strdup(h->text);
	sh_new_strdup(h->integer);
	sh_new_strdup(h->handler);
	sh_new_strdup(h->data);

	shdefault(h->integer, MwDEFAULT);
	shdefault(h->text, NULL);
	shdefault(h->handler, NULL);
	shdefault(h->data, NULL);

	h->prop_event = 0;
	if(MwDispatch2(h, create) != 0) {
		h->widget_class = NULL;
		MwDestroyWidget(h);
		return NULL;
	}
	h->prop_event = 1;

	if(h->widget_class != NULL && h->widget_class->tick != NULL) {
		MwAddTickList(h);
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
	int	 i;
	MwWidget root = handle;

	handle->destroyed = 0;
	MwDispatch(handle, destroy);

	for(i = 0; i < arrlen(handle->children); i++) {
		MwFreeWidget(handle->children[i]);
	}

	while(root->parent != NULL) root = root->parent;
	for(i = 0; i < arrlen(root->tick_list); i++) {
		if(handle == root->tick_list[i]) {
			arrdel(root->tick_list, i);
			i--;
		}
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
	arrfree(handle->tick_list);

	free(handle);
}

void MwDestroyWidget(MwWidget handle) {
	if(handle->parent != NULL) {
		arrput(handle->parent->destroy_queue, handle);
	}
	handle->destroyed = 1;
}

void MwVaWidgetExecute(MwWidget handle, const char* func_name, void* out, ...) {
	va_list args;
	va_start(args, out);
	MwVaListWidgetExecute(handle, func_name, out, args);
	va_end(args);
}

void MwVaListWidgetExecute(MwWidget handle, const char* func_name, void* out, va_list va) {
	if(handle->widget_class->execute != NULL) {
		handle->widget_class->execute(handle, func_name, out, va);
	}
}

static void clean_destroy_queue(MwWidget handle) {
	int i, j;
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

int MwStep(MwWidget handle) {
	int i;
	if(setjmp(handle->before_step)) return 0;
	for(i = 0; i < arrlen(handle->children); i++) MwStep(handle->children[i]);

	handle->prop_event = 0;
	if(handle->lowlevel != NULL && MwLLPending(handle->lowlevel)) MwLLNextEvent(handle->lowlevel);
	handle->prop_event = 1;

	clean_destroy_queue(handle);
	if(handle->parent == NULL && handle->destroyed) return 1;
	return 0;
}

int MwPending(MwWidget handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) {
		if(MwPending(handle->children[i])) return 1;
	}
	return (arrlen(handle->destroy_queue) > 0 ? 1 : 0) || (handle->widget_class == NULL ? 0 : MwLLPending(handle->lowlevel));
}

void MwLoop(MwWidget handle) {
	long tick = MwTimeGetTick();
	int  i;
	long wait = MwGetInteger(handle, MwNwaitMS);
	long over = 0;
	if(wait == MwDEFAULT) wait = MwWaitMS;
	while(!handle->close) {
		int  v = 0;
		long t, t2;
		long more;
		while(MwPending(handle)) {
			if((v = MwStep(handle)) != 0) break;
		}
		if(v != 0) break;

		for(i = 0; i < arrlen(handle->tick_list); i++) {
			MwDispatch(handle->tick_list[i], tick);
			MwDispatchUserHandler(handle->tick_list[i], MwNtickHandler, NULL);
		}

		more = over % (wait / 2);
		t    = (tick + wait - more) - (t2 = MwTimeGetTick());
		if(t > 0) {
			MwTimeSleep(t);
			tick = MwTimeGetTick();
			over -= more;
		} else {
			tick = t2;
			over += -t;
		}
	}
}

void MwSetInteger(MwWidget handle, const char* key, int n) {
	int xy;
	int wh = 0;
	if((xy = ((strcmp(key, MwNx) == 0 || strcmp(key, MwNy) == 0) ? 1 : 0)) || (wh = ((strcmp(key, MwNwidth) == 0 || strcmp(key, MwNheight) == 0)) ? 1 : 0)) {
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
		char* v = MwStringDupliacte(value);

		if(shgeti(handle->text, key) != -1) free(shget(handle->text, key));

		shput(handle->text, key, v);
	}
	if(handle->prop_event) MwDispatch3(handle, prop_change, key);
	if(strcmp(key, MwNbackground) == 0) {
		MwLLColor c = MwParseColor(handle, value);
		MwLLSetBackground(handle->lowlevel, c);
		MwLLFreeColor(c);
	}
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
		return MwDEFAULT;
	} else {
		return shget(handle->integer, key);
	}
}

const char* MwGetText(MwWidget handle, const char* key) {
	if(shgeti(handle->text, key) == -1 && (strcmp(key, MwNbackground) == 0 || strcmp(key, MwNforeground) == 0)) {
		const char* v = NULL;
		MwWidget    h = handle->parent;
		while(h != NULL) {
			if((v = MwGetText(h, key)) != NULL) break;
			h = h->parent;
		}
		if(v == NULL) {
			if(handle->dark_theme) {
				if(strcmp(key, MwNbackground) == 0) return MwDefaultDarkBackground;
				if(strcmp(key, MwNforeground) == 0) return MwDefaultDarkForeground;
			} else {
				if(strcmp(key, MwNbackground) == 0) return MwDefaultBackground;
				if(strcmp(key, MwNforeground) == 0) return MwDefaultForeground;
			}
		}
		return v;
	}

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

static void inherit_integer(MwWidget handle, const char* key, int default_value) {
	int	 n;
	MwWidget h = handle;
	while(h != NULL) {
		if((n = MwGetInteger(h, key)) != MwDEFAULT) {
			MwSetInteger(handle, key, n);
			return;
		}
		h = h->parent;
	}
	MwSetInteger(handle, key, default_value);
}

#if defined(USE_STB_TRUETYPE) || defined(USE_FREETYPE2)
static void set_font(MwWidget handle) {
	void*	 f;
	MwWidget h = handle;
	while(h != NULL) {
		if((f = MwGetVoid(h, MwNfont)) != NULL) {
			MwSetVoid(handle, MwNfont, f);
			return;
		}
		h = h->parent;
	}
	f = MwFontLoad(MwTTFData, MwTTFDataSize);
	MwSetVoid(handle, MwNfont, f);
}

static void set_boldfont(MwWidget handle) {
	void*	 f;
	MwWidget h = handle;
	while(h != NULL) {
		if((f = MwGetVoid(h, MwNboldFont)) != NULL) {
			MwSetVoid(handle, MwNboldFont, f);
			return;
		}
		h = h->parent;
	}
	f = MwFontLoad(MwBoldTTFData, MwBoldTTFDataSize);
	MwSetVoid(handle, MwNboldFont, f);
}
#endif

void MwSetDefault(MwWidget handle) {
	MwLLSetCursor(handle->lowlevel, &MwCursorDefault, &MwCursorDefaultMask);

#ifdef MW_CLASSIC_THEME
	inherit_integer(handle, MwNmodernLook, 0);
#else
	inherit_integer(handle, MwNmodernLook, 1);
#endif
#if defined(USE_STB_TRUETYPE) || defined(USE_FREETYPE2)
	set_font(handle);
	set_boldfont(handle);
#endif
}

void MwHideCursor(MwWidget handle) {
	MwLLSetCursor(handle->lowlevel, &MwCursorHidden, &MwCursorHiddenMask);
}

void MwDispatchUserHandler(MwWidget handle, const char* key, void* handler_data) {
	int ind = shgeti(handle->handler, key);
	if(ind == -1) return;
	if(handle->destroyed) return;

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

void MwForceRender2(MwWidget handle, void* ptr) {
	(void)ptr;

	MwForceRender(handle);
}

void MwAddTickList(MwWidget handle) {
	MwWidget root = handle;
	while(root->parent != NULL) root = root->parent;

	arrput(root->tick_list, handle);
}

void MwFocus(MwWidget handle) {
	MwLLFocus(handle->lowlevel);
}

void MwGrabPointer(MwWidget handle, int toggle) {
	if(toggle) MwFocus(handle);
	MwLLGrabPointer(handle->lowlevel, toggle);
}

static void force_render_all(MwWidget handle) {
	int i;
	for(i = 0; i < arrlen(handle->children); i++) {
		force_render_all(handle->children[i]);
	}
	MwForceRender(handle);
}

void MwToggleDarkTheme(MwWidget handle, int toggle) {
	int old = handle->dark_theme;
	if(old != toggle) {
		handle->dark_theme = toggle;

		force_render_all(handle);
	}
}

MwWidget MwGetParent(MwWidget handle) {
	return handle->parent;
}

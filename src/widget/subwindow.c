#include <Mw/Milsko.h>

#define TitleHeight 20
#define ButtonSize 14

static void resize(MwWidget handle){
	MwSubWindow sw = handle->internal;
	int x, y, w, h;
	int ww = MwGetInteger(handle, MwNwidth);
	int wh = MwGetInteger(handle, MwNheight);
	int bw = MwDefaultBorderWidth(handle);
	int p;

	x = bw * 2;
	y = bw * 2 + TitleHeight;
	w = ww - bw * 4;
	h = wh - bw * 4 - TitleHeight;
	if(sw->frame == NULL){
		sw->frame = MwCreateWidget(MwFrameClass, "frame", handle, x, y, w, h);
	}else{
		MwVaApply(sw->frame,
			MwNx, x,
			MwNy, y,
			MwNwidth, w,
			MwNheight, h,
		NULL);
	}

	p = (TitleHeight - ButtonSize) / 2;
	x = ww - bw - p;
	y = bw + p;
	w = ButtonSize;
	h = ButtonSize;

#define BUTTON(name) \
	x -= ButtonSize; \
	if(sw->name == NULL){ \
		sw->name = MwCreateWidget(MwButtonClass, #name, handle, x, y, w, h); \
	}else{ \
		MwVaApply(sw->name, \
			MwNx, x, \
			MwNy, y, \
			MwNwidth, w, \
			MwNheight, h, \
		NULL); \
	} \
	x -= p;

	BUTTON(close);
	BUTTON(maximize);
	BUTTON(minimize);

	MwSetText(sw->frame, MwNbackground, "#f00");
}

static int wcreate(MwWidget handle) {
	MwSubWindow sw = malloc(sizeof(*sw));
	memset(sw, 0, sizeof(*sw));

	handle->internal = sw;

	resize(handle);

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	MwSubWindow sw = handle->internal;
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwLLColor c  = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor tb = MwParseColor(handle, MwGetText(handle, MwNtitleBackground));
	MwLLColor tf = MwParseColor(handle, MwGetText(handle, MwNtitleForeground));
	MwRect	  r, r2;
	const char* title = MwGetText(handle, MwNtitle);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, c, 0);
	MwDrawRect(handle, &r, c);

	r2	  = r;
	r2.height = TitleHeight;
	MwDrawRect(handle, &r2, tb);

	if(title != NULL){
		MwPoint p;
		p.x = r2.x + (TitleHeight - ButtonSize);
		p.y = r2.y + TitleHeight / 2;
		handle->bgcolor = tb;
		MwDrawText(handle, NULL, &p, title, MwALIGNMENT_BEGINNING, tf);
		handle->bgcolor = NULL;
	}

	r.y += TitleHeight;
	r.height -= TitleHeight;
	MwDrawFrame(handle, &r, c, 1);

	MwLLFreeColor(tf);
	MwLLFreeColor(tb);
	MwLLFreeColor(c);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNtitle) == 0) MwForceRender(handle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	(void)out;
}

MwClassRec MwSubWindowClassRec = {
    wcreate,	  /* create */
    destroy,	  /* destroy */
    draw,	  /* draw */
    NULL,	  /* click */
    NULL,	  /* parent_resize */
    prop_change,  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* execute */
    NULL,	  /* tick */
    NULL,	  /* resize */
    NULL,	  /* children_update */
    NULL,	  /* children_prop_change */
    NULL,	  /* clipboard */
    NULL,	  /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwSubWindowClass = &MwSubWindowClassRec;

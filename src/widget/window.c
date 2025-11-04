/* $Id$ */
#include <Mw/Milsko.h>

static int create(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor c = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwRect	  r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawRect(handle, &r, c);

	MwLLFreeColor(c);
}
static void mwWindowMakeBorderlessImpl(MwWidget handle, int toggle) {
	MwLLMakeBorderless(handle->lowlevel, toggle);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	(void)out;

	if(strcmp(name, "mwWindowMakeBorderless") == 0) {
		int toggle = va_arg(va, int);
		mwWindowMakeBorderlessImpl(handle, toggle);
	}
}
MwClassRec MwWindowClassRec = {
    create,	  /* create */
    NULL,	  /* destroy */
    draw,	  /* draw */
    NULL,	  /* click */
    NULL,	  /* parent_resize */
    NULL,	  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* execute */
    NULL,	  /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwWindowClass = &MwWindowClassRec;

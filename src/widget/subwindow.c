#include <Mw/Milsko.h>

#define TitleHeight 20

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor c = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor c2 = MwParseColor(handle, MwGetText(handle, MwNsubBackground));
	MwRect	  r, r2;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawFrame(handle, &r, c, 0);
	MwDrawRect(handle, &r, c);

	r2 = r;
	r2.height = TitleHeight;
	MwDrawRect(handle, &r2, c2);

	r.y += TitleHeight;
	r.height -= TitleHeight;
	MwDrawFrame(handle, &r, c, 1);

	MwLLFreeColor(c2);
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
    NULL,	  /* destroy */
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

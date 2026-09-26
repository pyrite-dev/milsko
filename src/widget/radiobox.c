#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	MwSetInteger(handle, MwNchecked, 0);

	return 0;
}

static void draw(MwWidget handle) {
	MwRect	 r;
	MwColor	 base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwPixmap bgpx = MwGetVoid(handle, MwNbackgroundPixmap);

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);
	if(bgpx != NULL) MwDrawPixmap(handle, &r, bgpx);

	if(r.width < r.height) {
		r.height = r.width;
	} else {
		r.width = r.height;
	}
	r.x = (MwGetInteger(handle, MwNwidth) - r.width) / 2;
	r.y = (MwGetInteger(handle, MwNheight) - r.height) / 2;

	if(MwGetInteger(handle, MwNmodernLook) == 1) {
		int	is_checked = (handle->pressed || MwGetInteger(handle, MwNchecked));
		MwColor darker;
		MwColor inner;
		MwColor innerunsel = base;

		if(MwGetInteger(handle, MwNdarkTheme) == 1) {
			darker	   = MwAllocColor(handle, 255, 255, 255);
			innerunsel = MwLightenColor(handle, base, 80, 80, 80);
			inner	   = darker;
		} else {
			darker = MwAllocColor(handle, 0, 0, 0);
			inner  = MwLightenColor(handle, is_checked ? darker : base, -80, -80, -80);
		}
		MwDrawCircle(handle, &r, darker, NULL, 1);

		if(!MwGetInteger(handle, MwNdisabled)) {
			r.x += 2;
			r.y += 2;
			r.width -= 4;
			r.height -= 4;
			if(!is_checked) {
				MwDrawCircle(handle, &r, innerunsel, innerunsel, 1);
			} else {
				MwDrawCircle(handle, &r, inner, innerunsel, 1);
			}
		}
		MwFreeColor(darker);
	} else {
		MwDrawDiamond(handle, &r, base, (handle->pressed || MwGetInteger(handle, MwNchecked)) ? 1 : 0);
	}

	MwFreeColor(base);
}

static void click(MwWidget handle) {
	if(!MwGetInteger(handle, MwNchecked)) MwSetInteger(handle, MwNchecked, 1);
	if(MwGetInteger(handle, MwNchecked) && handle->parent != NULL) {
		int i;
		for(i = 0; i < arrlen(handle->parent->children); i++) {
			MwWidget w = handle->parent->children[i];
			if(w != handle && w->widget_class == MwRadioBoxClass && MwGetInteger(w, MwNchecked)) {
				MwSetInteger(w, MwNchecked, 0);
			}
		}
	}

	MwDispatchUserHandler(handle, MwNchangedHandler, NULL);
}
static void mouse_down(MwWidget handle, void* ptr) {
	handle->held = 1;
	MwForceRender2(handle, ptr);
}

static void mouse_up(MwWidget handle, void* ptr) {
	handle->held = 0;
	MwForceRender2(handle, ptr);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNchecked) == 0) MwForceRender(handle);
}

MwClassRec MwRadioBoxClassRec = {
    wcreate,	 /* create */
    NULL,	 /* destroy */
    draw,	 /* draw */
    click,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    mouse_up,	 /* mouse_up */
    mouse_down,	 /* mouse_down */
    NULL,	 /* key */
    NULL,	 /* execute */
    NULL,	 /* tick */
    NULL,	 /* resize */
    NULL,	 /* children_update */
    NULL,	 /* children_prop_change */
    NULL,	 /* clipboard */
    NULL,	 /* props_change */
    NULL,
    NULL,
    NULL};
MwClass MwRadioBoxClass = &MwRadioBoxClassRec;

#include <Mw/Milsko.h>

#include "../../external/md4c.h"

static int wcreate(MwWidget handle) {
	MwSetDefault(handle);

	return 0;
}

static void draw(MwWidget handle) {
	MwLLColor base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwRect	  r;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);
	MwDrawRect(handle, &r, base);

	MwLLFreeColor(base);
}

static void prop_change(MwWidget handle, const char* key) {
	if(strcmp(key, MwNautoResize) == 0 || strcmp(key, MwNtext) == 0) MwForceRender(handle);

	if(strcmp(key, MwNtext) == 0) {
		MD_PARSER parser;
		memset(&parser, 0, sizeof(parser));
	}
}

MwClassRec MwDocumentClassRec = {
    wcreate,	 /* create */
    NULL,	 /* destroy */
    draw,	 /* draw */
    NULL,	 /* click */
    NULL,	 /* parent_resize */
    prop_change, /* prop_change */
    NULL,	 /* mouse_move */
    NULL,	 /* mouse_up */
    NULL,	 /* mouse_down */
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
MwClass MwDocumentClass = &MwDocumentClassRec;

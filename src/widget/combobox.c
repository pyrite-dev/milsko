#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

static int create(MwWidget handle) {
	MwComboBox cb = malloc(sizeof(*cb));

	cb->list	 = NULL;
	cb->opened	 = 0;
	cb->listbox	 = NULL;
	handle->internal = cb;

	MwSetDefault(handle);

	MwSetInteger(handle, MwNareaShown, 6);
	MwSetInteger(handle, MwNvalue, 0);

	return 0;
}

static void destroy(MwWidget handle) {
	MwComboBox cb = handle->internal;
	int	   i;

	for(i = 0; i < arrlen(cb->list); i++) {
		free(cb->list[i]);
	}
	arrfree(cb->list);
	free(handle->internal);
}

static void draw(MwWidget handle) {
	MwRect	   r, rc;
	MwLLColor  base = MwParseColor(handle, MwGetText(handle, MwNbackground));
	MwLLColor  text = MwParseColor(handle, MwGetText(handle, MwNforeground));
	MwComboBox cb	= handle->internal;

	r.x	 = 0;
	r.y	 = 0;
	r.width	 = MwGetInteger(handle, MwNwidth);
	r.height = MwGetInteger(handle, MwNheight);

	MwDrawWidgetBack(handle, &r, base, 0, 1);

	rc = r;

	/* draw text */
	if(arrlen(cb->list) > MwGetInteger(handle, MwNvalue)) {
		MwPoint p;

		p.x = MwDefaultBorderWidth(handle) * 2 + 4;
		p.y = MwGetInteger(handle, MwNheight) / 2;

		MwDrawText(handle, &p, cb->list[MwGetInteger(handle, MwNvalue)], 0, MwALIGNMENT_BEGINNING, text);
	}

	r = rc;
	r.height /= 5;
	r.width	 = r.height * 3;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - MwDefaultBorderWidth(handle) * 2;
	r.y	 = MwDefaultBorderWidth(handle);
	r.height = rc.height;
	r.width += MwDefaultBorderWidth(handle) * 2;
	MwDrawWidgetBack(handle, &r, base, 0, 0);

	r	 = rc;
	r.width	 = r.height * 3 / 5;
	r.height = r.width - MwDefaultBorderWidth(handle) * 2;
	r.x	 = MwGetInteger(handle, MwNwidth) - MwDefaultBorderWidth(handle) - r.width - MwDefaultBorderWidth(handle) * 2;
	r.y	 = (MwGetInteger(handle, MwNheight) - (r.height + MwDefaultBorderWidth(handle) * 2)) / 2;
	MwDrawTriangle(handle, &r, base, cb->opened, MwSOUTH);

	r.y += r.height;
	r.height = MwDefaultBorderWidth(handle) * 2;
	MwDrawFrame(handle, &r, base, 0);

	MwLLFreeColor(text);
	MwLLFreeColor(base);
}

static void listbox_activate(MwWidget handle, void* user, void* client) {
	MwComboBox cb = handle->parent->internal;

	(void)user;

	MwSetInteger(handle->parent, MwNvalue, *(int*)client);
	cb->opened  = 0;
	cb->listbox = NULL;

	MwForceRender(handle->parent);

	MwDispatchUserHandler(handle->parent, MwNchangedHandler, client);

	MwDestroyWidget(handle);
}

static void click(MwWidget handle) {
	MwComboBox cb = handle->internal;

	cb->opened = cb->opened ? 0 : 1;
	if(cb->opened) {
		MwPoint p;
		int	i;
		void*	packet;
		int	width = MwGetInteger(handle, MwNwidth);
		int	ent   = MwGetInteger(handle, MwNareaShown);

		if(arrlen(cb->list) == 0) {
			cb->opened = 0;
			return;
		}

		if(ent > arrlen(cb->list)) ent = arrlen(cb->list);

		MwLLSetCursor(handle->lowlevel, &MwCursorArrow, &MwCursorArrowMask);

		for(i = 0; i < arrlen(cb->list); i++) {
			int l = MwTextWidth(handle, cb->list[i]) + MwDefaultBorderWidth(handle) * 2;
			if(l > width) width = l;
		}

		cb->listbox = MwVaCreateWidget(MwListBoxClass, "listbox", handle, 0, MwGetInteger(handle, MwNheight), width, MwTextHeight(handle, "M") * ent + MwDefaultBorderWidth(handle) * 2 + MwTextHeight(handle, "M") / 4,
					       MwNsingleClickSelectable, 1,
					       NULL);

		MwLLSetCursor(((MwListBox)cb->listbox->internal)->frame->lowlevel, &MwCursorArrow, &MwCursorArrowMask);

		packet = MwListBoxCreatePacket();
		for(i = 0; i < arrlen(cb->list); i++) {
			int index = MwListBoxPacketInsert(packet, -1);
			MwListBoxPacketSet(packet, index, 0, cb->list[i]);
		}
		MwListBoxInsert(cb->listbox, -1, packet);
		MwListBoxDestroyPacket(packet);

		MwAddUserHandler(cb->listbox, MwNactivateHandler, listbox_activate, NULL);

		p.x = 0;
		p.y = MwGetInteger(handle, MwNheight);
		MwLLBeginStateChange(cb->listbox->lowlevel);
		MwLLDetach(cb->listbox->lowlevel, &p);
		MwLLMakeToolWindow(cb->listbox->lowlevel);
		MwLLEndStateChange(cb->listbox->lowlevel);
	} else {
		MwLLSetCursor(handle->lowlevel, &MwCursorDefault, &MwCursorDefaultMask);

		if(cb->listbox != NULL) {
			MwDestroyWidget(cb->listbox);
			cb->listbox = NULL;
		}
	}

	MwForceRender(handle);
}

static void prop_change(MwWidget handle, const char* prop) {
	if(strcmp(prop, MwNvalue) == 0) {
		MwForceRender(handle);
	}
}

static void mwComboBoxAddImpl(MwWidget handle, int index, const char* text) {
	MwComboBox cb = handle->internal;
	char*	   t  = MwStringDuplicate(text);

	if(index == -1) index = arrlen(cb->list);

	arrins(cb->list, index, t);

	if(index <= MwGetInteger(handle, MwNvalue)) MwForceRender(handle);
}

static const char* mwComboBoxGetImpl(MwWidget handle, int index) {
	MwComboBox cb = handle->internal;

	return cb->list[index];
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwComboBoxAdd") == 0) {
		int	    index = va_arg(va, int);
		const char* text  = va_arg(va, const char*);
		mwComboBoxAddImpl(handle, index, text);
	}
	if(strcmp(name, "mwComboBoxGet") == 0) {
		int index	   = va_arg(va, int);
		*(const char**)out = mwComboBoxGetImpl(handle, index);
	}
}

MwClassRec MwComboBoxClassRec = {
    create,	    /* create */
    destroy,	    /* destroy */
    draw,	    /* draw */
    click,	    /* click */
    NULL,	    /* parent_resize */
    prop_change,    /* prop_change */
    NULL,	    /* mouse_move */
    MwForceRender2, /* mouse_up */
    MwForceRender2, /* mouse_down */
    NULL,	    /* key */
    func_handler,   /* execute */
    NULL,	    /* tick */
    NULL,	    /* resize */
    NULL,	    /* children_update */
    NULL,	    /* children_prop_change */
    NULL,	    /* clipboard */
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwComboBoxClass = &MwComboBoxClassRec;

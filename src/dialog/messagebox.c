/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

typedef struct msgbox {
	int	 key;
	MwWidget value;
} msgbox_t;

static void spawn_button(MwWidget handle, int x, int y, int id, const char* text) {
	msgbox_t* mb	 = handle->opaque;
	MwWidget  widget = MwVaCreateWidget(MwButtonClass, text, handle, x, y, 80, 24,
					    MwNtext, text,
					    NULL);

	hmput(mb, id, widget);

	handle->opaque = mb;
}

static void messagebox_close(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;
	MwMessageBoxDestroy(handle);
}

MwWidget MwMessageBox(MwWidget handle, const char* text, const char* title, unsigned int flag) {
	MwWidget window;
	MwPoint	 p;
	int	 w, h;
	int	 left = 8;
	int	 th;
	int	 x = 0;
	int	 wx;
	int	 wy;
	int	 ww = handle == NULL ? 0 : MwGetInteger(handle, MwNwidth);
	int	 wh = handle == NULL ? 0 : MwGetInteger(handle, MwNheight);
	MwSizeHints sh;

	w = 512;
	h = 32 * 4;

	wx = wy = 0;
	if(handle == NULL) wx = wy = MwDEFAULT;

	sh.min_width = sh.max_width = w;
	sh.min_height = sh.max_height = h;

	p.x    = (ww - w) / 2;
	p.y    = (wh - h) / 2;
	window = MwVaCreateWidget(MwWindowClass, "messagebox", handle, wx, wy, w, h,
				  MwNtitle, title,
				  MwNsizeHints, &sh,
				  NULL);

	window->opaque = NULL;

	if(flag & MwMB_BUTTONOK) spawn_button(window, w - (x += 8 + 80), h - 8 - 24, MwMB_BUTTONOK, "OK");
	if(flag & MwMB_BUTTONCANCEL) spawn_button(window, w - (x += 8 + 80), h - 8 - 24, MwMB_BUTTONCANCEL, "Cancel");

	if(flag & MwMB_BUTTONNO) spawn_button(window, w - (x += 8 + 80), h - 8 - 24, MwMB_BUTTONNO, "No");
	if(flag & MwMB_BUTTONYES) spawn_button(window, w - (x += 8 + 80), h - 8 - 24, MwMB_BUTTONYES, "Yes");

	if((flag & MwMB_ICONMASK) != 0) {
		MwWidget      icon;
		MwLLPixmap    px;
		unsigned int* data = NULL;

		icon = MwCreateWidget(MwImageClass, "image", window, 8, (h - 48) / 2, 48, 48);

		switch(flag & MwMB_ICONMASK) {
		case MwMB_ICONWARNING: {
			data = MwIconWarning;
			break;
		}
		case MwMB_ICONINFO: {
			data = MwIconInfo;
			break;
		}
		case MwMB_ICONNOTE: {
			data = MwIconNote;
			break;
		}
		case MwMB_ICONNEWS: {
			data = MwIconNews;
			break;
		}
		case MwMB_ICONERROR: {
			data = MwIconError;
			break;
		}
		case MwMB_ICONCLOCK: {
			data = MwIconClock;
			break;
		}
		}

		px = MwLoadIcon(icon, data);

		MwSetVoid(icon, MwNpixmap, px);

		left = 8 + 48 + 8;
	}

	th = MwTextHeight(handle, text);
	(void)MwVaCreateWidget(MwLabelClass, "label", window, left, (h - th) / 2, w - left - 8, th,
			       MwNtext, text,
			       MwNalignment, MwALIGNMENT_BEGINNING,
			       NULL);

	MwLLBeginStateChange(window->lowlevel);
	if(handle != NULL) MwLLDetach(window->lowlevel, &p);
	MwLLMakePopup(window->lowlevel, handle == NULL ? NULL : handle->lowlevel);
	MwLLEndStateChange(window->lowlevel);

	MwAddUserHandler(window, MwNcloseHandler, messagebox_close, NULL);

	return window;
}

MwWidget MwMessageBoxGetChild(MwWidget handle, int child) {
	msgbox_t* mb = handle->opaque;

	return hmget(mb, child);
}

void MwMessageBoxDestroy(MwWidget handle) {
	void* px;

	if((px = MwGetVoid(handle, MwNpixmap)) != NULL) MwLLDestroyPixmap(px);
	MwDestroyWidget(handle);
}

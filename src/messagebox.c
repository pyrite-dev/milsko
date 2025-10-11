/* $Id$ */
#include <Mw/Milsko.h>

void msgbox_ok_handler(MwWidget handle, void* user, void* call) {
	MwWidget win = user;

	MwDestroyWidget(win);
}

MwWidget MwMessageBox(MwWidget handle, const char* text, const char* title, unsigned int flag) {
	MwWidget window, ok;
	MwPoint	 p;
	int	 w, h;
	int	 left = 8;
	int	 th;

	p.x = 0;
	p.y = 0;

	window = MwVaCreateWidget(MwWindowClass, "messagebox", handle, 0, 0, (w = 512), (h = 32 * 4),
				  MwNtitle, title,
				  NULL);
	ok     = MwVaCreateWidget(MwButtonClass, "ok", window, w - 8 - 80, h - 8 - 24, 80, 24,
				  MwNtext, "OK",
				  NULL);

	MwAddUserHandler(ok, MwNactivateHandler, msgbox_ok_handler, window);

	if((flag & MwMB_ICONMASK) != 0) {
		MwWidget   icon;
		MwLLPixmap px;
		char**	   data = NULL;

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
		case MwMB_ICONQUESTION: {
			data = MwIconQuestion;
			break;
		}
		}

		px = MwLoadXPM(icon, data);

		MwSetVoid(icon, MwNpixmap, px);

		left = 8 + 48 + 8;
	}

	th = MwTextHeight(handle, text);
	(void)MwVaCreateWidget(MwLabelClass, "label", window, left, (h - th) / 2, w - left - 8, th,
			       MwNtext, text,
			       MwNalignment, MwALIGNMENT_BEGINNING,
			       NULL);

	MwLLDetach(window->lowlevel, &p);
	MwLLSetSizeHints(window->lowlevel, w, h, w, h);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);

	return window;
}

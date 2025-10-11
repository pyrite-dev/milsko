/* $Id$ */
#include <Mw/Milsko.h>

MwWidget MwMessageBox(MwWidget handle, const char* text, const char* title, unsigned int flag) {
	MwWidget window, ok;
	MwPoint	 p;
	int	 w, h;

	p.x = 0;
	p.y = 0;

	window = MwVaCreateWidget(MwWindowClass, "messagebox", handle, 0, 0, (w = 512), (h = 32 * 4),
				  MwNtitle, title,
				  NULL);
	ok     = MwVaCreateWidget(MwButtonClass, "ok", window, w - 8 - 64, h - 8 - 24, 64, 24,
				  MwNtext, "OK",
				  NULL);

	if((flag & MwMB_ICONMASK) != 0) {
		MwWidget   icon;
		MwLLPixmap px;
		char**	   data = NULL;

		icon = MwCreateWidget(MwImageClass, "image", window, 16, (h - 48) / 2, 48, 48);

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
	}

	MwLLDetach(window->lowlevel, &p);
	MwLLSetSizeHints(window->lowlevel, w, h, w, h);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);

	return window;
}

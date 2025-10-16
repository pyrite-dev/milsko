/* $Id$ */
#include <Mw/Milsko.h>

MwWidget MwFileChooser(MwWidget handle, const char* title){
	MwWidget window;
	MwPoint	 p;
	int	 ww = MwGetInteger(handle, MwNwidth);
	int	 wh = MwGetInteger(handle, MwNheight);
	int	 w, h;

	p.x = 0;
	p.y = 0;

	window = MwVaCreateWidget(MwWindowClass, "filechooser", handle, ww, wh, (w = 640), (h = 300),
		MwNtitle, title,
	NULL);

	MwLLDetach(window->lowlevel, &p);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);
	
	return window;
}

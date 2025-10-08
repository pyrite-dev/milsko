/* $Id$ */
#ifndef __MWOO_WIDGET_WINDOW_H__
#define __MWOO_WIDGET_WINDOW_H__

#include <MwOO/Base.h>

namespace MwOO {
	class Window : public MwOO::Base {
	public:
		Window(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	};
}

#endif

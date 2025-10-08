/* $Id$ */
#ifndef __MWOO_WIDGET_MENU_H__
#define __MWOO_WIDGET_MENU_H__

#include <MwOO/Base.h>

namespace MwOO {
	class Menu : public MwOO::Base {
	public:
		Menu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
		MwMenu Add(MwMenu menu, const char* name);
	};
}

#endif

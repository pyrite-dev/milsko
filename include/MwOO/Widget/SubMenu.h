/* $Id$ */
#ifndef __MWOO_WIDGET_SUBMENU_H__
#define __MWOO_WIDGET_SUBMENU_H__

#include <MwOO/Base.h>

namespace MwOO {
class SubMenu : public MwOO::Base {
      public:
	SubMenu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

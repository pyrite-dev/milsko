/* $Id$ */
#ifndef __MWOO_WIDGET_TEXT_H__
#define __MWOO_WIDGET_TEXT_H__

#include <MwOO/Base.h>

namespace MwOO {
class Text : public MwOO::Base {
      public:
	Text(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetChecked(int value);
	int	    GetChecked(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

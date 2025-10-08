/* $Id$ */
#ifndef __MWOO_WIDGET_LABEL_H__
#define __MWOO_WIDGET_LABEL_H__

#include <MwOO/Base.h>

namespace MwOO {
class Label : public MwOO::Base {
      public:
	Label(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetText(const char* value);
	const char* GetText(void);
	void	    SetAlignment(int value);
	int	    GetAlignment(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

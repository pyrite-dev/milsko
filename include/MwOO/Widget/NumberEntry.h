/* $Id$ */
#ifndef __MWOO_WIDGET_NUMBERENTRY_H__
#define __MWOO_WIDGET_NUMBERENTRY_H__

#include <MwOO/Base.h>

namespace MwOO {
class NumberEntry : public MwOO::Base {
      public:
	NumberEntry(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetText(const char* value);
	const char* GetText(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

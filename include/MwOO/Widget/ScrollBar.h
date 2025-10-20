/* $Id$ */
#ifndef __MWOO_WIDGET_SCROLLBAR_H__
#define __MWOO_WIDGET_SCROLLBAR_H__

#include <MwOO/Base.h>

namespace MwOO {
class ScrollBar : public MwOO::Base {
      public:
	ScrollBar(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetAreaShown(int value);
	int	    GetAreaShown(void);
	void	    SetValue(int value);
	int	    GetValue(void);
	void	    SetMinValue(int value);
	int	    GetMinValue(void);
	void	    SetMaxValue(int value);
	int	    GetMaxValue(void);
	void	    SetOrientation(int value);
	int	    GetOrientation(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

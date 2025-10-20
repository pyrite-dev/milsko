/* $Id$ */
#ifndef __MWOO_WIDGET_WINDOW_H__
#define __MWOO_WIDGET_WINDOW_H__

#include <MwOO/Base.h>

namespace MwOO {
class Window : public MwOO::Base {
      public:
	Window(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    MakeBorderless(int toggle);
	void	    SetTitle(const char* value);
	const char* GetTitle(void);
	void	    SetMain(int value);
	int	    GetMain(void);
	void	    SetIconPixmap(void* value);
	void*	    GetIconPixmap(void);
	void	    SetSizeHints(void* value);
	void*	    GetSizeHints(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

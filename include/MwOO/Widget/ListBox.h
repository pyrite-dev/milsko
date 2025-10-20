/* $Id$ */
#ifndef __MWOO_WIDGET_LISTBOX_H__
#define __MWOO_WIDGET_LISTBOX_H__

#include <MwOO/Base.h>

namespace MwOO {
class ListBox : public MwOO::Base {
      public:
	ListBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    Insert(int index, MwLLPixmap pixmap, ...);
	void	    InsertMultiple(int index, int count, MwLLPixmap* pixmap, ...);
	void	    VaInsert(int index, MwLLPixmap pixmap, va_list va);
	void	    VaInsertMultiple(int index, int count, MwLLPixmap* pixmap, va_list va);
	void	    SetLeftPadding(int value);
	int	    GetLeftPadding(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

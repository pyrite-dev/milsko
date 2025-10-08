/* $Id$ */
#ifndef __MWOO_WIDGET_IMAGE_H__
#define __MWOO_WIDGET_IMAGE_H__

#include <MwOO/Base.h>

namespace MwOO {
class Image : public MwOO::Base {
      public:
	Image(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    SetPixmap(void* value);
	void*	    GetPixmap(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

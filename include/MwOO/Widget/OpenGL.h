/* $Id$ */
#ifndef __MWOO_WIDGET_OPENGL_H__
#define __MWOO_WIDGET_OPENGL_H__

#include <MwOO/Base.h>

namespace MwOO {
class OpenGL : public MwOO::Base {
      public:
	OpenGL(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	void	    MakeCurrent(void);
	void*	    GetProcAddress(const char* name);
	void	    SwapBuffer(void);
	void	    SetBackground(const char* value);
	const char* GetBackground(void);
	void	    SetForeground(const char* value);
	const char* GetForeground(void);
};
} // namespace MwOO

#endif

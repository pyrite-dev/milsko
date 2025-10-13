/* $Id$ */
#ifndef __MWOO_BASE_H__
#define __MWOO_BASE_H__

#include <Mw/Milsko.h>

#define MwOODeclare(name, parent_class) \
      public: \
	name(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : parent_class(widget_name, parent, x, y, w, h) {}

namespace MwOO {
class Base {
      protected:
	MwWidget widget;

      private:
	int  can_be_gc;
	void SetHandler(void);

      public:
	MwOO::Base MessageBox(const char* title, const char* text, unsigned int flags);
	MwOO::Base MessageBoxGetChild(int num);
	void	   Loop(void);
	void	   SetX(int value);
	void	   SetY(int value);
	void	   SetWidth(int value);
	void	   SetHeight(int value);
	int	   GetX(void);
	int	   GetY(void);
	int	   GetWidth(void);
	int	   GetHeight(void);
	/* BEGIN AUTOGENERATE */
	virtual void OnActivate(void*) {};
	virtual void OnResize(void*) {};
	virtual void OnTick(void*) {};
	virtual void OnMenu(void*) {};
	virtual void OnMouseDownHandler(void*) {};
	virtual void OnMouseUpHandler(void*) {};
	virtual void OnMouseMoveHandler(void*) {};
	virtual void OnChangedHandler(void*) {};
	virtual void OnKeyHandler(void*) {};
	virtual void OnKeyReleaseHandler(void*) {};
	virtual void OnCloseHandler(void*) {};

	/* END AUTOGENERATE */

	Base(MwClass widget_class, const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	Base(MwWidget widget, int gc = 0);
	~Base(void);
};
} // namespace MwOO

#endif

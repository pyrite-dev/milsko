/* $Id$ */
#ifndef __MWOO_BASE_H__
#define __MWOO_BASE_H__

#include <Mw/Milsko.h>

class MwOOWidget {
      protected:
	MwWidget widget;

      public:
	MwOOWidget(MwClass widget_class, const char* name, MwOOWidget* parent, int x, int y, int w, int h);
	~MwOOWidget(void);
};

#endif

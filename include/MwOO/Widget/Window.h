/* $Id$ */
#ifndef __MWOO_WIDGET_WINDOW_H__
#define __MWOO_WIDGET_WINDOW_H__

#include <MwOO/Base.h>

class MwOOWindowWidget : public MwOOWidget {
      public:
	MwOOWindowWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h);
};

#endif

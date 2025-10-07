/* $Id$ */
#ifndef __MWOO_WIDGET_FRAME_H__
#define __MWOO_WIDGET_FRAME_H__

#include <MwOO/Base.h>

class MwOOFrameWidget : public MwOOWidget {
      public:
	MwOOFrameWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h);
};

#endif

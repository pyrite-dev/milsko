/* $Id$ */
#ifndef __MWOO_WIDGET_SCROLLBAR_H__
#define __MWOO_WIDGET_SCROLLBAR_H__

#include <MwOO/Base.h>

namespace MwOO {
class ScrollBar : public MwOO::Base {
      public:
	ScrollBar(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
};
} // namespace MwOO

#endif

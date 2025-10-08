/* $Id$ */
#ifndef __MWOO_WIDGET_FRAME_H__
#define __MWOO_WIDGET_FRAME_H__

#include <MwOO/Base.h>

namespace MwOO {
	class Frame : public MwOO::Base {
	public:
		Frame(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
	};
}

#endif

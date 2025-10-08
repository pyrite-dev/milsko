/* $Id$ */
#ifndef __MWOO_WIDGET_BUTTON_H__
#define __MWOO_WIDGET_BUTTON_H__

#include <MwOO/Base.h>

namespace MwOO {
	class Button : public MwOO::Base {
	public:
		Button(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
		void SetPixmap(void* value);
		void* GetPixmap(void);
	};
}

#endif

/* $Id$ */
#ifndef __MWOO_WIDGET_VULKAN_H__
#define __MWOO_WIDGET_VULKAN_H__

#include <MwOO/Base.h>

namespace MwOO {
class Vulkan : public MwOO::Base {
      public:
	Vulkan(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);
};
} // namespace MwOO

#endif

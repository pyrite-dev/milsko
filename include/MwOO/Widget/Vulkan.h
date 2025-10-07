/* $Id$ */
#ifndef __MWOO_WIDGET_VULKAN_H__
#define __MWOO_WIDGET_VULKAN_H__

#include <MwOO/Base.h>

class MwOOVulkanWidget : public MwOOWidget {
      public:
	MwOOVulkanWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h);
};

#endif

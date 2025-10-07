/* $Id$ */
#include <MwOO/Widget/Vulkan.h>
#include <Mw/Widget/Vulkan.h>

MwOOVulkanWidget::MwOOVulkanWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwVulkanClass;
}

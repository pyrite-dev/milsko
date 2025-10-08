/* $Id$ */
#include <MwOO/Widget/Vulkan.h>
#include <Mw/Widget/Vulkan.h>

MwOO::Vulkan::Vulkan(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwVulkanClass, widget_name, parent, x, y, w, h){
}

void MwOO::Vulkan::EnableExtension(void){
	MwVulkanEnableExtension(this->widget);
}
void MwOO::Vulkan::EnableLayer(void){
	MwVulkanEnableLayer(this->widget);
}
void MwOO::Vulkan::Configure(void){
	MwVulkanConfigure(this->widget);
}
void* MwOO::Vulkan::GetField(MwVulkanField field, MwErrorEnum* out){
	return MwVulkanGetField(this->widget, field, out);
}
VkBool32 MwOO::Vulkan::Supported(void){
	return MwVulkanSupported(this->widget);
}
void MwOO::Vulkan::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Vulkan::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Vulkan::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Vulkan::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


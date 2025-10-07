/* $Id$ */
#include <MwOO/Widget/Image.h>
#include <Mw/Widget/Image.h>

MwOO::Image::Image(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwImageClass, widget_name, parent, x, y, w, h){
}

void MwOO::Image::SetPixmap(void* value){
	MwSetVoid(this->widget, MwNpixmap, value);
}

void* MwOO::Image::GetPixmap(void){
	return MwGetVoid(this->widget, MwNpixmap);
}


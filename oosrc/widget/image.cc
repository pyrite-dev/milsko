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

void MwOO::Image::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Image::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Image::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Image::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


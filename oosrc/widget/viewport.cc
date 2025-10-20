/* $Id$ */
#include <MwOO/Widget/Viewport.h>
#include <Mw/Widget/Viewport.h>

MwOO::Viewport::Viewport(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwViewportClass, widget_name, parent, x, y, w, h){
}

void MwOO::Viewport::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Viewport::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Viewport::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Viewport::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


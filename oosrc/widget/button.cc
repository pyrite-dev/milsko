/* $Id$ */
#include <MwOO/Widget/Button.h>
#include <Mw/Widget/Button.h>

MwOO::Button::Button(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwButtonClass, widget_name, parent, x, y, w, h){
}

void MwOO::Button::SetPixmap(void* value){
	MwSetVoid(this->widget, MwNpixmap, value);
}

void* MwOO::Button::GetPixmap(void){
	return MwGetVoid(this->widget, MwNpixmap);
}

void MwOO::Button::SetText(const char* value){
	MwSetText(this->widget, MwNtext, value);
}

const char* MwOO::Button::GetText(void){
	return MwGetText(this->widget, MwNtext);
}


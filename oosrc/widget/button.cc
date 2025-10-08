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

void MwOO::Button::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Button::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Button::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Button::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


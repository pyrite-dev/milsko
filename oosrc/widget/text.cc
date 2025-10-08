/* $Id$ */
#include <MwOO/Widget/Text.h>
#include <Mw/Widget/Text.h>

MwOO::Text::Text(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwTextClass, widget_name, parent, x, y, w, h){
}

void MwOO::Text::SetChecked(int value){
	MwSetInteger(this->widget, MwNchecked, value);
}

int MwOO::Text::GetChecked(void){
	return MwGetInteger(this->widget, MwNchecked);
}

void MwOO::Text::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Text::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Text::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Text::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


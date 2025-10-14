/* $Id$ */
#include <MwOO/Widget/ListBox.h>
#include <Mw/Widget/ListBox.h>

MwOO::ListBox::ListBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwListBoxClass, widget_name, parent, x, y, w, h){
}

void MwOO::ListBox::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::ListBox::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::ListBox::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::ListBox::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


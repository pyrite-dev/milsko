/* $Id$ */
#include <MwOO/Widget/Menu.h>
#include <Mw/Widget/Menu.h>

MwOO::Menu::Menu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwMenuClass, widget_name, parent, x, y, w, h){
}

void MwOO::Menu::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Menu::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Menu::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Menu::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


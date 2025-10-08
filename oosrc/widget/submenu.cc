/* $Id$ */
#include <MwOO/Widget/SubMenu.h>
#include <Mw/Widget/SubMenu.h>

MwOO::SubMenu::SubMenu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwSubMenuClass, widget_name, parent, x, y, w, h){
}

void MwOO::SubMenu::Appear(MwMenu menu, MwPoint* point){
	MwSubMenuAppear(this->widget, menu, point);
}
void MwOO::SubMenu::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::SubMenu::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::SubMenu::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::SubMenu::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


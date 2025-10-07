/* $Id$ */
#include <MwOO/Widget/SubMenu.h>
#include <Mw/Widget/SubMenu.h>

MwOO::SubMenu::SubMenu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwSubMenuClass, widget_name, parent, x, y, w, h){
}

void MwOO::SubMenu::Appear(MwMenu menu, MwPoint* point){
	MwSubMenuAppear(this->widget, menu, point);
}

/* $Id$ */
#include <MwOO/Widget/SubMenu.h>
#include <Mw/Widget/SubMenu.h>

MwOOSubMenuWidget::MwOOSubMenuWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwSubMenuClass;
}

/* $Id$ */
#include <MwOO/Widget/Menu.h>
#include <Mw/Widget/Menu.h>

MwOOMenuWidget::MwOOMenuWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwMenuClass;
}

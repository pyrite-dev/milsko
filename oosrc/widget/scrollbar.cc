/* $Id$ */
#include <MwOO/Widget/ScrollBar.h>
#include <Mw/Widget/ScrollBar.h>

MwOOScrollBarWidget::MwOOScrollBarWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwScrollBarClass;
}

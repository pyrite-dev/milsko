/* $Id$ */
#include <MwOO/Widget/Window.h>
#include <Mw/Widget/Window.h>

MwOOWindowWidget::MwOOWindowWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwWindowClass;
}

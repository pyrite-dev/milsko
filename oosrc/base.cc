/* $Id$ */
#include <MwOO/Base.h>

MwOOWidget::MwOOWidget(MwClass widget_class, const char* name, MwOOWidget* parent, int x, int y, int w, int h){
	this->widget = MwCreateWidget(widget_class, name, parent->widget, x, y, w, h);
}

MwOOWidget::~MwOOWidget(void){
	MwDestroyWidget(this->widget);
}

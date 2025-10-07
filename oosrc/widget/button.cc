/* $Id$ */
#include <MwOO/Widget/Button.h>
#include <Mw/Widget/Button.h>

MwOOButtonWidget::MwOOButtonWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwButtonClass;
}

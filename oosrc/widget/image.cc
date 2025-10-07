/* $Id$ */
#include <MwOO/Widget/Image.h>
#include <Mw/Widget/Image.h>

MwOOImageWidget::MwOOImageWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwImageClass;
}

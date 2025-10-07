/* $Id$ */
#include <MwOO/Widget/Frame.h>
#include <Mw/Widget/Frame.h>

MwOOFrameWidget::MwOOFrameWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwFrameClass;
}

/* $Id$ */
#include <MwOO/Widget/OpenGL.h>
#include <Mw/Widget/OpenGL.h>

MwOOOpenGLWidget::MwOOOpenGLWidget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(name, parent, x, y, w, h){
	this->widget_class = MwOpenGLClass;
}

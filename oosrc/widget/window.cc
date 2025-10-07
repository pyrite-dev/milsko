/* $Id$ */
#include <MwOO/Widget/Window.h>
#include <Mw/Widget/Window.h>

MwOO::Window::Window(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwWindowClass, widget_name, parent, x, y, w, h){
}

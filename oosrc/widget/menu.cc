/* $Id$ */
#include <MwOO/Widget/Menu.h>
#include <Mw/Widget/Menu.h>

MwOO::Menu::Menu(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwMenuClass, widget_name, parent, x, y, w, h){
}

MwMenu MwOO::Menu::Add(MwMenu menu, const char* name){
	return MwMenuAdd(this->widget, menu, name);
}

/* $Id$ */
#include <MwOO/Widget/CheckBox.h>
#include <Mw/Widget/CheckBox.h>

MwOO::CheckBox::CheckBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwCheckBoxClass, widget_name, parent, x, y, w, h){
}

void MwOO::CheckBox::SetPixmap(void* value){
	MwSetVoid(this->widget, MwNpixmap, value);
}

void* MwOO::CheckBox::GetPixmap(void){
	return MwGetVoid(this->widget, MwNpixmap);
}


/* $Id$ */
#include <MwOO/Widget/CheckBox.h>
#include <Mw/Widget/CheckBox.h>

MwOO::CheckBox::CheckBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwCheckBoxClass, widget_name, parent, x, y, w, h){
}

void MwOO::CheckBox::SetChecked(int value){
	MwSetInteger(this->widget, MwNchecked, value);
}

int MwOO::CheckBox::GetChecked(void){
	return MwGetInteger(this->widget, MwNchecked);
}


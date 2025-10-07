/* $Id$ */
#include <MwOO/Widget/ScrollBar.h>
#include <Mw/Widget/ScrollBar.h>

MwOO::ScrollBar::ScrollBar(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwScrollBarClass, widget_name, parent, x, y, w, h){
}

int MwOO::ScrollBar::GetVisibleLength(void){
	return MwScrollBarGetVisibleLength(this->widget);
}
void MwOO::ScrollBar::SetAreaShown(int value){
	MwSetInteger(this->widget, MwNareaShown, value);
}

int MwOO::ScrollBar::GetAreaShown(void){
	return MwGetInteger(this->widget, MwNareaShown);
}

void MwOO::ScrollBar::SetValue(int value){
	MwSetInteger(this->widget, MwNvalue, value);
}

int MwOO::ScrollBar::GetValue(void){
	return MwGetInteger(this->widget, MwNvalue);
}

void MwOO::ScrollBar::SetMinValue(int value){
	MwSetInteger(this->widget, MwNminValue, value);
}

int MwOO::ScrollBar::GetMinValue(void){
	return MwGetInteger(this->widget, MwNminValue);
}

void MwOO::ScrollBar::SetMaxValue(int value){
	MwSetInteger(this->widget, MwNmaxValue, value);
}

int MwOO::ScrollBar::GetMaxValue(void){
	return MwGetInteger(this->widget, MwNmaxValue);
}

void MwOO::ScrollBar::SetOrientation(int value){
	MwSetInteger(this->widget, MwNorientation, value);
}

int MwOO::ScrollBar::GetOrientation(void){
	return MwGetInteger(this->widget, MwNorientation);
}


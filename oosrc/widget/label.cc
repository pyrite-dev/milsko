/* $Id$ */
#include <MwOO/Widget/Label.h>
#include <Mw/Widget/Label.h>

MwOO::Label::Label(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwLabelClass, widget_name, parent, x, y, w, h){
}

void MwOO::Label::SetText(const char* value){
	MwSetText(this->widget, MwNtext, value);
}

const char* MwOO::Label::GetText(void){
	return MwGetText(this->widget, MwNtext);
}

void MwOO::Label::SetAlignment(int value){
	MwSetInteger(this->widget, MwNalignment, value);
}

int MwOO::Label::GetAlignment(void){
	return MwGetInteger(this->widget, MwNalignment);
}

void MwOO::Label::SetBold(int value){
	MwSetInteger(this->widget, MwNbold, value);
}

int MwOO::Label::GetBold(void){
	return MwGetInteger(this->widget, MwNbold);
}

void MwOO::Label::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Label::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Label::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Label::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


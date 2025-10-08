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


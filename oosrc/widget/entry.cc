/* $Id$ */
#include <MwOO/Widget/Entry.h>
#include <Mw/Widget/Entry.h>

MwOO::Entry::Entry(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwEntryClass, widget_name, parent, x, y, w, h){
}

void MwOO::Entry::SetText(const char* value){
	MwSetText(this->widget, MwNtext, value);
}

const char* MwOO::Entry::GetText(void){
	return MwGetText(this->widget, MwNtext);
}

void MwOO::Entry::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Entry::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Entry::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Entry::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


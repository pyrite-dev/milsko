/* $Id$ */
#include <MwOO/Widget/NumberEntry.h>
#include <Mw/Widget/NumberEntry.h>

MwOO::NumberEntry::NumberEntry(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwNumberEntryClass, widget_name, parent, x, y, w, h){
}

void MwOO::NumberEntry::SetText(const char* value){
	MwSetText(this->widget, MwNtext, value);
}

const char* MwOO::NumberEntry::GetText(void){
	return MwGetText(this->widget, MwNtext);
}

void MwOO::NumberEntry::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::NumberEntry::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::NumberEntry::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::NumberEntry::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


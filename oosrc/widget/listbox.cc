/* $Id$ */
#include <MwOO/Widget/ListBox.h>
#include <Mw/Widget/ListBox.h>

MwOO::ListBox::ListBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwListBoxClass, widget_name, parent, x, y, w, h){
}

void MwOO::ListBox::Insert(int index, MwLLPixmap pixmap, ...){
	va_list va;
	va_start(va, pixmap);
	MwListBoxVaInsert(this->widget, index, pixmap, va);
	va_end(va);
}

void MwOO::ListBox::InsertMultiple(int index, int count, MwLLPixmap* pixmap, ...){
	va_list va;
	va_start(va, pixmap);
	MwListBoxVaInsertMultiple(this->widget, index, count, pixmap, va);
	va_end(va);
}

void MwOO::ListBox::VaInsert(int index, MwLLPixmap pixmap, va_list va){
	MwListBoxVaInsert(this->widget, index, pixmap, va);
}

void MwOO::ListBox::VaInsertMultiple(int index, int count, MwLLPixmap* pixmap, va_list va){
	MwListBoxVaInsertMultiple(this->widget, index, count, pixmap, va);
}

void MwOO::ListBox::SetLeftPadding(int value){
	MwSetInteger(this->widget, MwNleftPadding, value);
}

int MwOO::ListBox::GetLeftPadding(void){
	return MwGetInteger(this->widget, MwNleftPadding);
}

void MwOO::ListBox::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::ListBox::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::ListBox::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::ListBox::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


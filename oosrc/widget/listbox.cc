/* $Id$ */
#include <MwOO/Widget/ListBox.h>
#include <Mw/Widget/ListBox.h>

MwOO::ListBox::ListBox(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwListBoxClass, widget_name, parent, x, y, w, h){
}

void MwOO::ListBox::Insert(int index, const char* text, MwLLPixmap pixmap){
	MwListBoxInsert(this->widget, index, text, pixmap);
}
void MwOO::ListBox::InsertMultiple(int index, char** text, MwLLPixmap* pixmap, int count){
	MwListBoxInsertMultiple(this->widget, index, text, pixmap, count);
}
void MwOO::ListBox::Delete(int index){
	MwListBoxDelete(this->widget, index);
}
const char* MwOO::ListBox::Get(int index){
	return MwListBoxGet(this->widget, index);
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


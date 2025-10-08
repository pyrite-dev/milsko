/* $Id$ */
#include <MwOO/Widget/Frame.h>
#include <Mw/Widget/Frame.h>

MwOO::Frame::Frame(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwFrameClass, widget_name, parent, x, y, w, h){
}

void MwOO::Frame::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Frame::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Frame::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Frame::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


/* $Id$ */
#include <MwOO/Widget/Window.h>
#include <Mw/Widget/Window.h>

MwOO::Window::Window(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwWindowClass, widget_name, parent, x, y, w, h){
}

void MwOO::Window::MakeBorderless(int toggle){
	MwWindowMakeBorderless(this->widget, toggle);
}
void MwOO::Window::SetTitle(const char* value){
	MwSetText(this->widget, MwNtitle, value);
}

const char* MwOO::Window::GetTitle(void){
	return MwGetText(this->widget, MwNtitle);
}

void MwOO::Window::SetMain(int value){
	MwSetInteger(this->widget, MwNmain, value);
}

int MwOO::Window::GetMain(void){
	return MwGetInteger(this->widget, MwNmain);
}

void MwOO::Window::SetIconPixmap(void* value){
	MwSetVoid(this->widget, MwNiconPixmap, value);
}

void* MwOO::Window::GetIconPixmap(void){
	return MwGetVoid(this->widget, MwNiconPixmap);
}

void MwOO::Window::SetSizeHints(void* value){
	MwSetVoid(this->widget, MwNsizeHints, value);
}

void* MwOO::Window::GetSizeHints(void){
	return MwGetVoid(this->widget, MwNsizeHints);
}

void MwOO::Window::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::Window::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::Window::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::Window::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


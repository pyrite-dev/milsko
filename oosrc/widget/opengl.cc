/* $Id$ */
#include <MwOO/Widget/OpenGL.h>
#include <Mw/Widget/OpenGL.h>

MwOO::OpenGL::OpenGL(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(MwOpenGLClass, widget_name, parent, x, y, w, h){
}

void MwOO::OpenGL::MakeCurrent(void){
	MwOpenGLMakeCurrent(this->widget);
}
void* MwOO::OpenGL::GetProcAddress(const char* name){
	return MwOpenGLGetProcAddress(this->widget, name);
}
void MwOO::OpenGL::SwapBuffer(void){
	MwOpenGLSwapBuffer(this->widget);
}
void MwOO::OpenGL::SetBackground(const char* value){
	MwSetText(this->widget, MwNbackground, value);
}

const char* MwOO::OpenGL::GetBackground(void){
	return MwGetText(this->widget, MwNbackground);
}

void MwOO::OpenGL::SetForeground(const char* value){
	MwSetText(this->widget, MwNforeground, value);
}

const char* MwOO::OpenGL::GetForeground(void){
	return MwGetText(this->widget, MwNforeground);
}


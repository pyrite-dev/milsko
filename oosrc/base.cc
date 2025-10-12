/* $Id$ */
#include <MwOO/Base.h>

MwOO::Base::Base(MwClass widget_class, const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h){
	this->widget = MwCreateWidget(widget_class, widget_name, parent == NULL ? NULL : parent->widget, x, y, w, h);
	this->can_be_gc = 1;
	if(this->widget != NULL){
		this->SetHandler();
	}
}

MwOO::Base::Base(MwWidget widget, int gc){
	this->widget = widget;
	this->can_be_gc = gc;
	this->SetHandler();
}

MwOO::Base::~Base(void){
	if(this->can_be_gc) MwDestroyWidget(this->widget);
}

MwOO::Base MwOO::Base::MessageBox(const char* title, const char* text, unsigned int flags){
	MwWidget widget = MwMessageBox(this->widget, title, text, flags);

	return MwOO::Base(widget, 1);
}

MwOO::Base MwOO::Base::MessageBoxGetChild(int num){
	MwWidget widget = MwMessageBoxGetChild(this->widget, num);

	return MwOO::Base(widget);
}

void MwOO::Base::Loop(void){
	MwLoop(this->widget);
}

void MwOO::Base::SetX(int value){
	MwSetInteger(this->widget, MwNx, value);
}

void MwOO::Base::SetY(int value){
	MwSetInteger(this->widget, MwNy, value);
}

void MwOO::Base::SetWidth(int value){
	MwSetInteger(this->widget, MwNwidth, value);
}

void MwOO::Base::SetHeight(int value){
	MwSetInteger(this->widget, MwNheight, value);
}

int MwOO::Base::GetX(void){
	return MwGetInteger(this->widget, MwNx);
}

int MwOO::Base::GetY(void){
	return MwGetInteger(this->widget, MwNy);
}

int MwOO::Base::GetWidth(void){
	return MwGetInteger(this->widget, MwNwidth);
}

int MwOO::Base::GetHeight(void){
	return MwGetInteger(this->widget, MwNheight);
}

/* BEGIN AUTOGENERATE */
static void __OnActivate(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnActivate(call);
}
static void __OnResize(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnResize(call);
}
static void __OnTick(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnTick(call);
}
static void __OnMenu(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnMenu(call);
}
static void __OnMouseDownHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnMouseDownHandler(call);
}
static void __OnMouseUpHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnMouseUpHandler(call);
}
static void __OnChangedHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnChangedHandler(call);
}
static void __OnKeyHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnKeyHandler(call);
}
static void __OnKeyReleaseHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnKeyReleaseHandler(call);
}
static void __OnCloseHandler(MwWidget widget, void* user, void* call){
	MwOO::Base* c = (MwOO::Base*)user;
	c->OnCloseHandler(call);
}

void MwOO::Base::SetHandler(void){
	MwAddUserHandler(this->widget, MwNactivateHandler, __OnActivate, this);
	MwAddUserHandler(this->widget, MwNresizeHandler, __OnResize, this);
	MwAddUserHandler(this->widget, MwNtickHandler, __OnTick, this);
	MwAddUserHandler(this->widget, MwNmenuHandler, __OnMenu, this);
	MwAddUserHandler(this->widget, MwNmouseDownHandler, __OnMouseDownHandler, this);
	MwAddUserHandler(this->widget, MwNmouseUpHandler, __OnMouseUpHandler, this);
	MwAddUserHandler(this->widget, MwNchangedHandler, __OnChangedHandler, this);
	MwAddUserHandler(this->widget, MwNkeyHandler, __OnKeyHandler, this);
	MwAddUserHandler(this->widget, MwNkeyReleaseHandler, __OnKeyReleaseHandler, this);
	MwAddUserHandler(this->widget, MwNcloseHandler, __OnCloseHandler, this);
}
/* END AUTOGENERATE */

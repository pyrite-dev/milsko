/* $Id$ */
#include <MwOO/Base.h>

MwOO::Base::Base(MwClass widget_class, const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h){
	this->widget = MwCreateWidget(widget_class, widget_name, parent == NULL ? NULL : parent->widget, x, y, w, h);
	if(this->widget != NULL){
		this->SetHandler();
	}
}

MwOO::Base::~Base(void){
	MwDestroyWidget(this->widget);
}

void MwOO::Base::Loop(void){
	MwLoop(this->widget);
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

void MwOO::Base::SetHandler(void){
	MwAddUserHandler(this->widget, MwNactivateHandler, __OnActivate, this);
	MwAddUserHandler(this->widget, MwNresizeHandler, __OnResize, this);
	MwAddUserHandler(this->widget, MwNtickHandler, __OnTick, this);
	MwAddUserHandler(this->widget, MwNmenuHandler, __OnMenu, this);
}
/* END AUTOGENERATE */

/* $Id$ */

#include <Mw/Milsko.h>

#include <GL/gl.h>

MwWidget window, opengl;
int ow, oh;

void resize(MwWidget handle, void* user_data, void* call_data){
	unsigned int w, h;

	(void)user_data;
	(void)call_data;

	w = MwGetInteger(handle, MwNwidth);
	h = MwGetInteger(handle, MwNheight);

	MwVaApply(opengl,
		MwNwidth, (ow = w - 100),
		MwNheight, (oh = h - 100),
	NULL);
}

void tick(MwWidget handle, void* user_data, void* call_data){
	(void)handle;
	(void)user_data;
	(void)call_data;

	MwOpenGLMakeCurrent(opengl);

	glViewport(0, 0, ow, oh);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0); glVertex2f(0, 0.8);
		glColor3f(0, 1, 0); glVertex2f(-0.8, -0.8);
		glColor3f(0, 0, 1); glVertex2f(0.8, -0.8);
	glEnd();

	MwOpenGLSwapBuffer(opengl);
}

int main(){
	window = MwVaCreateWidget(MwWindowClass, "main", NULL, 0, 0, 400, 400,
		MwNtitle, "hello world",
	NULL);
	opengl = MwCreateWidget(MwOpenGLClass, "opengl", window, 50, 50, (ow = 300), (oh = 300));

	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(window, MwNtickHandler, tick, NULL);

	MwLoop(window);
}

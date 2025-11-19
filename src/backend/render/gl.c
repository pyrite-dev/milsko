/* $Id$ */
#include "Mw/LowLevel.h"
#include "Mw/Widget/OpenGL.h"
#include <Mw/Milsko.h>
#include <assert.h>
#include <stdio.h>

typedef struct gl_funcs_t {
	void (*glClear)(GLbitfield mask);
	void (*glFlush)(void);
	void (*glBegin)(GLenum mode);
	void (*glEnd)(void);
	void (*glColor3f)(GLfloat red, GLfloat green, GLfloat blue);
	void (*glVertex2i)(GLint x, GLint y);
} gl_funcs;
static gl_funcs get_gl_funcs(MwLL handle) {
	gl_funcs g = {
	    .glClear	= MwLLGLGetProcAddress(handle, handle->common.gl, "glClear"),
	    .glFlush	= MwLLGLGetProcAddress(handle, handle->common.gl, "glFlush"),
	    .glBegin	= MwLLGLGetProcAddress(handle, handle->common.gl, "glBegin"),
	    .glEnd	= MwLLGLGetProcAddress(handle, handle->common.gl, "glEnd"),
	    .glColor3f	= MwLLGLGetProcAddress(handle, handle->common.gl, "glColor3f"),
	    .glVertex2i = MwLLGLGetProcAddress(handle, handle->common.gl, "glVertex2i"),
	};
	return g;
}

static void MwLLPolygonImplGL(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int i = 0;
	MwLLGLMakeCurrent(handle, handle->common.gl);

	if(handle == NULL) {
		return;
	}
	if(handle->common.gl == NULL) {
		return;
	}

	gl_funcs g = get_gl_funcs(handle);
	g.glClear(GL_COLOR_BUFFER_BIT);
	g.glBegin(GL_TRIANGLES);
	for(i = 0; i < points_count; i++) {
		g.glColor3f(color->common.red / 255., color->common.green / 255., color->common.blue / 255.);
		g.glVertex2i(points[i].x, points[i].y);
	}
	g.glEnd();
	g.glFlush();

	MwLLGLSwapBuffer(handle, handle->common.gl);
}

static void MwLLLineImplGL(MwLL handle, MwPoint* points, MwLLColor color) {
	printf("line gl\n");
}

static MwLLColor MwLLAllocColorImplGL(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	MwLLColorUpdate(handle, c, r, g, b);
	// printf("alloc color gl\n");

	return c;
}

static void MwLLColorUpdateImplGL(MwLL handle, MwLLColor c, int r, int g, int b) {
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
	// printf("update color gl\n");
}

static void MwLLFreeColorImplGL(MwLLColor color) {
	// printf("free color gl\n");
	free(color);
}

static void MwLLSetBackgroundImplGL(MwLL handle, MwLLColor color) {
	(void)handle;
	(void)color;
	// printf("background gl\n");
}

static MwLLPixmap MwLLCreatePixmapImplGL(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));

	// printf("create pixmap gl\n");

	return r;
}

static void MwLLPixmapUpdateImplGL(MwLLPixmap r) {
	int y, x;
	int w = (r->common.width + (16 - (r->common.width % 16))) / 8;

	// printf("create pixmap gl\n");
}

static void MwLLDestroyPixmapImplGL(MwLLPixmap pixmap) {
	// printf("destroy pixmap gl\n");
}

static void MwLLDrawPixmapImplGL(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	printf("draw pixmap gl\n");
}

/* $Id$ */
#include <Mw/Milsko.h>
#include <Mw/OpenGL.h>

#ifdef _WIN32
/* nothing */
#else
#include <GL/glx.h>
#endif
#include <GL/gl.h>

#ifdef _WIN32
typedef struct opengl {
	HDC   dc;
	HGLRC gl;
} opengl_t;
#else
typedef struct opengl {
	XVisualInfo* visual;
	GLXContext   gl;
} opengl_t;
#endif

static void create(MwWidget handle) {
	opengl_t* o = malloc(sizeof(*o));
#ifdef _WIN32
	PIXELFORMATDESCRIPTOR pfd;
	int		      pf;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 32;

	o->dc = GetDC(handle->lowlevel->hWnd);

	pf = ChoosePixelFormat(o->dc, &pfd);
	SetPixelFormat(o->dc, pf, &pfd);

	o->gl = wglCreateContext(o->dc);
#else
	int attribs[5];

	attribs[0] = GLX_RGBA;
	attribs[1] = GLX_DOUBLEBUFFER;
	attribs[2] = GLX_DEPTH_SIZE;
	attribs[3] = 24;
	attribs[4] = None;

	/* XXX: fix this */
	o->visual = glXChooseVisual(handle->lowlevel->display, DefaultScreen(handle->lowlevel->display), attribs);
	o->gl	  = glXCreateContext(handle->lowlevel->display, o->visual, NULL, GL_TRUE);
#endif
	handle->internal	      = o;
	handle->lowlevel->copy_buffer = 0;

	MwSetDefault(handle);
}

static void destroy(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	wglMakeCurrent(NULL, NULL);
	DeleteDC(o->dc);
	wglDeleteContext(o->gl);
#else
	glXMakeCurrent(handle->lowlevel->display, None, NULL);
	glXDestroyContext(handle->lowlevel->display, o->gl);
#endif
	free(o);
}

MwClassRec MwOpenGLClassRec = {
    create,  /* create */
    destroy, /* destroy */
    NULL,    /* draw */
    NULL     /* click */
};
MwClass MwOpenGLClass = &MwOpenGLClassRec;

void MwOpenGLMakeCurrent(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	wglMakeCurrent(o->dc, o->gl);
#else
	glXMakeCurrent(handle->lowlevel->display, handle->lowlevel->window, o->gl);
#endif
}

void MwOpenGLSwapBuffer(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	SwapBuffers(o->dc);
#else
	(void)o;

	glXSwapBuffers(handle->lowlevel->display, handle->lowlevel->window);
#endif
}

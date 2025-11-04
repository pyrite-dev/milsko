/* $Id$ */
#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

typedef void(GLAPIENTRY* MWglColor3f)(GLfloat red, GLfloat green, GLfloat blue);
#ifdef _WIN32
typedef HGLRC(WINAPI* MWwglCreateContext)(HDC);
typedef BOOL(WINAPI* MWwglMakeCurrent)(HDC, HGLRC);
typedef PROC(WINAPI* MWwglGetProcAddress)(LPCSTR);
typedef BOOL(WINAPI* MWwglDeleteContext)(HGLRC);

typedef struct opengl {
	HDC   dc;
	HGLRC gl;

	void* lib;

	MWwglCreateContext  wglCreateContext;
	MWwglMakeCurrent    wglMakeCurrent;
	MWwglDeleteContext  wglDeleteContext;
	MWwglGetProcAddress wglGetProcAddress;
	MWglColor3f	    glColor3f;
} opengl_t;
#else
typedef XVisualInfo* (*MWglXChooseVisual)(Display* dpy, int screen, int* attribList);
typedef GLXContext (*MWglXCreateContext)(Display* dpy, XVisualInfo* vis, GLXContext shareList, Bool direct);
typedef void (*MWglXDestroyContext)(Display* dpy, GLXContext ctx);
typedef Bool (*MWglXMakeCurrent)(Display* dpy, GLXDrawable drawable, GLXContext ctx);
typedef void (*MWglXSwapBuffers)(Display* dpy, GLXDrawable drawable);
typedef void* (*MWglXGetProcAddress)(const GLubyte* procname);

typedef struct opengl {
	XVisualInfo* visual;
	GLXContext   gl;

	void* lib;

	MWglXChooseVisual   glXChooseVisual;
	MWglXCreateContext  glXCreateContext;
	MWglXDestroyContext glXDestroyContext;
	MWglXMakeCurrent    glXMakeCurrent;
	MWglXSwapBuffers    glXSwapBuffers;
	MWglXGetProcAddress glXGetProcAddress;
	MWglColor3f	    glColor3f;
} opengl_t;
#endif

static int create(MwWidget handle) {
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

	o->lib = LoadLibrary("opengl32.dll");

	o->wglCreateContext  = (MWwglCreateContext)(void*)GetProcAddress(o->lib, "wglCreateContext");
	o->wglMakeCurrent    = (MWwglMakeCurrent)(void*)GetProcAddress(o->lib, "wglMakeCurrent");
	o->wglDeleteContext  = (MWwglDeleteContext)(void*)GetProcAddress(o->lib, "wglDeleteContext");
	o->wglGetProcAddress = (MWwglGetProcAddress)(void*)GetProcAddress(o->lib, "wglGetProcAddress");
	o->glColor3f	     = (MWglColor3f)(void*)GetProcAddress(o->lib, "glColor3f");

	o->gl = o->wglCreateContext(o->dc);
#else
	int	    attribs[5];
	const char* glpath[] = {
	    "libGL.so",
	    "/usr/local/lib/libGL.so",
	    "/usr/X11R7/lib/libGL.so",
	    "/usr/pkg/lib/libGL.so"};
	int glincr = 0;

	attribs[0] = GLX_RGBA;
	attribs[1] = GLX_DOUBLEBUFFER;
	attribs[2] = GLX_DEPTH_SIZE;
	attribs[3] = 24;
	attribs[4] = None;

	while(glpath[glincr] != NULL && (o->lib = dlopen(glpath[glincr++], RTLD_LAZY)) == NULL);

	o->glXChooseVisual   = (MWglXChooseVisual)dlsym(o->lib, "glXChooseVisual");
	o->glXCreateContext  = (MWglXCreateContext)dlsym(o->lib, "glXCreateContext");
	o->glXDestroyContext = (MWglXDestroyContext)dlsym(o->lib, "glXDestroyContext");
	o->glXMakeCurrent    = (MWglXMakeCurrent)dlsym(o->lib, "glXMakeCurrent");
	o->glXSwapBuffers    = (MWglXSwapBuffers)dlsym(o->lib, "glXSwapBuffers");
	o->glXGetProcAddress = (MWglXGetProcAddress)dlsym(o->lib, "glXGetProcAddress");
	o->glColor3f	     = (MWglColor3f)dlsym(o->lib, "glColor3f");

	/* XXX: fix this */
	o->visual = o->glXChooseVisual(handle->lowlevel->display, DefaultScreen(handle->lowlevel->display), attribs);
	o->gl	  = o->glXCreateContext(handle->lowlevel->display, o->visual, NULL, GL_TRUE);
#endif
	handle->internal	      = o;
	handle->lowlevel->copy_buffer = 0;

	MwSetDefault(handle);

	return 0;
}

static void destroy(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	o->wglMakeCurrent(NULL, NULL);
	DeleteDC(o->dc);
	o->wglDeleteContext(o->gl);

	FreeLibrary(o->lib);
#else
	o->glXMakeCurrent(handle->lowlevel->display, None, NULL);
	o->glXDestroyContext(handle->lowlevel->display, o->gl);

	dlclose(o->lib);
#endif
	free(o);
}

static void mwOpenGLMakeCurrentImpl(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	o->wglMakeCurrent(o->dc, o->gl);
#else
	o->glXMakeCurrent(handle->lowlevel->display, handle->lowlevel->window, o->gl);
#endif
}

static void mwOpenGLSwapBufferImpl(MwWidget handle) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	SwapBuffers(o->dc);
#else
	(void)o;

	o->glXSwapBuffers(handle->lowlevel->display, handle->lowlevel->window);
#endif
}

static void* mwOpenGLGetProcAddressImpl(MwWidget handle, const char* name) {
	opengl_t* o = (opengl_t*)handle->internal;
#ifdef _WIN32
	return o->wglGetProcAddress(name);
#else
	return o->glXGetProcAddress((const GLubyte*)name);
#endif
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwOpenGLMakeCurrent") == 0) {
		mwOpenGLMakeCurrentImpl(handle);
	}
	if(strcmp(name, "mwOpenGLSwapBuffer") == 0) {
		mwOpenGLSwapBufferImpl(handle);
	}
	if(strcmp(name, "mwOpenGLGetProcAddress") == 0) {
		const char* _name = va_arg(va, const char*);
		*(void**)out	  = mwOpenGLGetProcAddressImpl(handle, _name);
	}
}

MwClassRec MwOpenGLClassRec = {
    create,	  /* create */
    destroy,	  /* destroy */
    NULL,	  /* draw */
    NULL,	  /* click */
    NULL,	  /* parent_resize */
    NULL,	  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* execute */
    NULL,	  /* tick */
    NULL,
    NULL,
    NULL};
MwClass MwOpenGLClass = &MwOpenGLClassRec;

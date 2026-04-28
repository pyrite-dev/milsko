#ifndef __APPLE__

#include <Mw/Milsko.h>

#ifndef MW_OPENGL
#define MW_OPENGL_NO_INCLUDE
#endif
#include <Mw/Widget/OpenGL.h>

#ifdef MW_OPENGL
#ifdef USE_GDI
typedef HGLRC(WINAPI* MWwglCreateContext)(HDC);
typedef BOOL(WINAPI* MWwglMakeCurrent)(HDC, HGLRC);
typedef PROC(WINAPI* MWwglGetProcAddress)(LPCSTR);
typedef BOOL(WINAPI* MWwglDeleteContext)(HGLRC);

typedef struct gdiopengl {
	HDC   dc;
	HGLRC gl;

	void* lib;

	MWwglCreateContext  wglCreateContext;
	MWwglMakeCurrent    wglMakeCurrent;
	MWwglDeleteContext  wglDeleteContext;
	MWwglGetProcAddress wglGetProcAddress;
} gdiopengl_t;
#endif
#ifdef USE_X11
typedef XVisualInfo* (*MWglXChooseVisual)(Display* dpy, int screen,
					  int* attribList);
typedef GLXContext (*MWglXCreateContext)(Display* dpy, XVisualInfo* vis,
					 GLXContext shareList, Bool direct);
typedef void (*MWglXDestroyContext)(Display* dpy, GLXContext ctx);
typedef Bool (*MWglXMakeCurrent)(Display* dpy, GLXDrawable drawable,
				 GLXContext ctx);
typedef void (*MWglXSwapBuffers)(Display* dpy, GLXDrawable drawable);
typedef void* (*MWglXGetProcAddress)(const GLubyte* procname);

typedef struct x11opengl {
	XVisualInfo* visual;
	GLXContext   gl;

	void* lib;

	MWglXChooseVisual   glXChooseVisual;
	MWglXCreateContext  glXCreateContext;
	MWglXDestroyContext glXDestroyContext;
	MWglXMakeCurrent    glXMakeCurrent;
	MWglXSwapBuffers    glXSwapBuffers;
	MWglXGetProcAddress glXGetProcAddress;
} x11opengl_t;
#endif

#ifdef USE_WAYLAND
#include <EGL/egl.h>
#include <wayland-egl-core.h>

typedef struct waylandopengl {
	EGLNativeWindowType egl_window_native;
	EGLDisplay	    egl_display;
	EGLContext	    egl_context;
	EGLSurface	    egl_surface;
	EGLConfig	    egl_config;

	void* gllib;
	void* wlgllib;
	EGLBoolean (*eglGetConfigs)(EGLDisplay display,
				    EGLConfig* configs,
				    EGLint     config_size,
				    EGLint*    num_config);
	EGLBoolean (*eglInitialize)(EGLDisplay display,
				    EGLint*    major,
				    EGLint*    minor);
	EGLContext (*eglCreateContext)(EGLDisplay    display,
				       EGLConfig     config,
				       EGLContext    share_context,
				       EGLint const* attrib_list);
	EGLBoolean (*eglMakeCurrent)(EGLDisplay display,
				     EGLSurface draw,
				     EGLSurface read,
				     EGLContext context);
	void* (*eglGetProcAddress)(char const* procname);
	EGLBoolean (*eglSwapBuffers)(EGLDisplay display,
				     EGLSurface surface);
	EGLBoolean (*eglSwapInterval)(EGLDisplay display,
				      EGLint	 interval);
	EGLint (*eglGetError)(void);
	EGLBoolean (*eglBindAPI)(EGLenum api);
	EGLDisplay (*eglGetDisplay)(NativeDisplayType native_display);
	EGLSurface (*eglCreateWindowSurface)(EGLDisplay	      display,
					     EGLConfig	      config,
					     NativeWindowType native_window,
					     EGLint const*    attrib_list);
	EGLBoolean (*eglChooseConfig)(EGLDisplay    display,
				      EGLint const* attrib_list,
				      EGLConfig*    configs,
				      EGLint	    config_size,
				      EGLint*	    num_config);

	struct wl_egl_window* (*wl_egl_window_create)(struct wl_surface* surface,
						      int width, int height);
	void (*wl_egl_window_resize)(struct wl_egl_window* egl_window,
				     int width, int height,
				     int dx, int dy);
} waylandopengl_t;
#endif

static int wcreate(MwWidget handle) {
	void*	 r = NULL;
	MwWidget w = handle;

#ifdef USE_GDI
	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		PIXELFORMATDESCRIPTOR pfd;
		int		      pf;
		gdiopengl_t*	      o = r = malloc(sizeof(*o));

		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize      = sizeof(pfd);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.cDepthBits = 32;
		pfd.cColorBits = 32;

		o->dc = GetDC(handle->lowlevel->gdi.hWnd);

		pf = ChoosePixelFormat(o->dc, &pfd);
		SetPixelFormat(o->dc, pf, &pfd);

		o->lib = MwDynamicOpen("opengl32.dll");
		if(!o->lib){
			printf("Could not load OpenGL widget under WGL! " #x " missing.\n"); \
			return 1;
		}

#define WGL_FUNC(x) o->x = MwDynamicSymbol(o->lib, #x); \
		if(o->x == NULL){ \
			printf("Could not load OpenGL widget under WGL! " #x " missing.\n"); \
			return 1; \
		}

		WGL_FUNC(wglCreateContext);
		WGL_FUNC(wglMakeCurrent);
		WGL_FUNC(wglDeleteContext);
		WGL_FUNC(wglGetProcAddress);

		o->gl = o->wglCreateContext(o->dc);
	}
#endif
#ifdef USE_X11
	if(handle->lowlevel->common.type == MwLLBackendX11) {
		int	     attribs[5];
		const char*  glpath[] = {"libGL.so", "/usr/local/lib/libGL.so",
					 "/usr/X11R7/lib/libGL.so", "/usr/pkg/lib/libGL.so"};
		int	     glincr   = 0;
		x11opengl_t* o = r = malloc(sizeof(*o));

		attribs[0] = GLX_RGBA;
		attribs[1] = GLX_DOUBLEBUFFER;
		attribs[2] = GLX_DEPTH_SIZE;
		attribs[3] = 24;
		attribs[4] = None;

		while(glpath[glincr] != NULL &&
		      (o->lib = MwDynamicOpen(glpath[glincr++])) == NULL);

		o->glXChooseVisual =
		    (MWglXChooseVisual)MwDynamicSymbol(o->lib, "glXChooseVisual");
		o->glXCreateContext =
		    (MWglXCreateContext)MwDynamicSymbol(o->lib, "glXCreateContext");
		o->glXDestroyContext =
		    (MWglXDestroyContext)MwDynamicSymbol(o->lib, "glXDestroyContext");
		o->glXMakeCurrent =
		    (MWglXMakeCurrent)MwDynamicSymbol(o->lib, "glXMakeCurrent");
		o->glXSwapBuffers =
		    (MWglXSwapBuffers)MwDynamicSymbol(o->lib, "glXSwapBuffers");
		o->glXGetProcAddress =
		    (MWglXGetProcAddress)MwDynamicSymbol(o->lib, "glXGetProcAddress");

		/* XXX: fix this */
		o->visual = o->glXChooseVisual(handle->lowlevel->x11.display,
					       DefaultScreen(handle->lowlevel->x11.display),
					       attribs);
		o->gl	  = o->glXCreateContext(handle->lowlevel->x11.display, o->visual, NULL,
						GL_TRUE);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		EGLint		 numConfigs;
		EGLint		 majorVersion;
		EGLint		 minorVersion;
		EGLContext	 context;
		EGLSurface	 surface;
		EGLint		 fbAttribs[]	  = {EGL_SURFACE_TYPE,
						     EGL_WINDOW_BIT,
						     EGL_RENDERABLE_TYPE,
						     EGL_OPENGL_ES2_BIT,
						     EGL_RED_SIZE,
						     8,
						     EGL_GREEN_SIZE,
						     8,
						     EGL_BLUE_SIZE,
						     8,
						     EGL_DEPTH_SIZE,
						     24,
						     EGL_RENDERABLE_TYPE,
						     EGL_OPENGL_BIT,
						     EGL_NONE};
		EGLint		 contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION,
						     1,
						     EGL_CONTEXT_MAJOR_VERSION,
						     1,
						     EGL_CONTEXT_MINOR_VERSION,
						     1,
						     EGL_NONE};
		EGLDisplay	 display;
		waylandopengl_t* o = r = malloc(sizeof(*o));
		MwWaylandAlwaysRender  = MwTRUE;

		o->gllib = MwDynamicOpen("libEGL.so");
		if(!o->gllib) {
			printf("Could not load OpenGL widget under Wayland! libEGL.so missing.\n");
			return 1;
		}
		o->wlgllib = MwDynamicOpen("libwayland-egl.so");
		if(!o->gllib) {
			printf("Could not load OpenGL widget under Wayland! libwayland-egl.so missing.\n");
			return 1;
		}
#define EGL_FUNC(x) \
	o->x = MwDynamicSymbol(o->gllib, #x); \
	if(!o->x) { \
		printf("Could not load OpenGL widget under Wayland! " #x " missing.\n"); \
		return 1; \
	};
#define WAYLAND_EGL_FUNC(x) \
	o->x = MwDynamicSymbol(o->wlgllib, #x); \
	if(!o->x) { \
		printf("Could not load OpenGL widget under Wayland! " #x " missing.\n"); \
		return 1; \
	};
		EGL_FUNC(eglGetConfigs)
		EGL_FUNC(eglInitialize)
		EGL_FUNC(eglCreateContext)
		EGL_FUNC(eglMakeCurrent)
		EGL_FUNC(eglGetProcAddress)
		EGL_FUNC(eglSwapBuffers)
		EGL_FUNC(eglSwapInterval)
		EGL_FUNC(eglGetError)
		EGL_FUNC(eglBindAPI)
		EGL_FUNC(eglGetDisplay)
		EGL_FUNC(eglCreateWindowSurface)
		EGL_FUNC(eglChooseConfig)
		WAYLAND_EGL_FUNC(wl_egl_window_create)
		WAYLAND_EGL_FUNC(wl_egl_window_resize)

#define eglGetConfigs o->eglGetConfigs
#define eglInitialize o->eglInitialize
#define eglCreateContext o->eglCreateContext
#define eglMakeCurrent o->eglMakeCurrent
#define eglGetProcAddress o->eglGetProcAddress
#define eglSwapBuffers o->eglSwapBuffers
#define eglSwapInterval o->eglSwapInterval
#define eglGetError o->eglGetError
#define wl_egl_window_create o->wl_egl_window_create
#define eglBindAPI o->eglBindAPI
#define eglGetDisplay o->eglGetDisplay
#define wl_egl_window_resize o->wl_egl_window_resize
#define eglCreateWindowSurface o->eglCreateWindowSurface
#define eglChooseConfig o->eglChooseConfig

		display =
		    eglGetDisplay((EGLNativeDisplayType)handle->lowlevel->wayland.display);
		if(display == EGL_NO_DISPLAY) {
			printf("ERROR: eglGetDisplay, %0X\n", eglGetError());
			return 1;
		}
		/* Initialize EGL */
		if(!eglInitialize(display, &majorVersion, &minorVersion)) {
			printf("ERROR: eglInitialize, %0X\n", eglGetError());
			return 1;
		}

		/* Get configs */
		if((eglGetConfigs(display, NULL, 0, &numConfigs) != EGL_TRUE) ||
		   (numConfigs == 0)) {
			printf("ERROR: eglGetConfigs, %0X\n", eglGetError());
			return 1;
		}

		/* Choose config */
		if((eglChooseConfig(display, fbAttribs, &o->egl_config, 1, &numConfigs) !=
		    EGL_TRUE) ||
		   (numConfigs != 1)) {
			printf("ERROR: eglChooseConfig, %0X\n", eglGetError());
			return 1;
		}

		o->egl_window_native = (EGLNativeWindowType)wl_egl_window_create(
		    handle->lowlevel->wayland.framebuffer.surface,
		    handle->lowlevel->wayland.ww, handle->lowlevel->wayland.wh);
		if(!o->egl_window_native) {
			printf("ERROR: wl_egl_window_create, EGL_NO_SURFACE\n");
			return 1;
		}

		/* Create a surface */
		surface = eglCreateWindowSurface(display, o->egl_config,
						 o->egl_window_native, NULL);
		if(surface == EGL_NO_SURFACE) {
			printf("ERROR: eglCreateWindowSurface, %0X\n", eglGetError());
			return 1;
		}

		eglBindAPI(EGL_OPENGL_API);

		/* Create a GL context */
		context = eglCreateContext(display, o->egl_config, EGL_NO_CONTEXT,
					   contextAttribs);
		if(context == EGL_NO_CONTEXT) {
			printf("ERROR: eglCreateContext, %0X\n", eglGetError());
			return 1;
		}

		if(!eglMakeCurrent(display, surface, surface, context)) {
			printf("ERROR: eglMakeCurrent (setup): %0X\n", eglGetError());
		}
		eglSwapInterval(o->egl_display, 0);

		o->egl_display = display;
		o->egl_surface = surface;
		o->egl_context = context;
	}
#endif

	handle->internal		     = r;
	handle->lowlevel->common.copy_buffer = 0;

	MwSetDefault(handle);

	while(w->parent != NULL)
		w = w->parent;

	w->berserk++;

	return 0;
}

static void destroy(MwWidget handle) {
	MwWidget w = handle;

#ifdef USE_GDI
	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		gdiopengl_t* o = handle->internal;

		o->wglMakeCurrent(NULL, NULL);
		DeleteDC(o->dc);
		o->wglDeleteContext(o->gl);

		MwDynamicClose(o->lib);
	}
#endif
#ifdef USE_X11
	if(handle->lowlevel->common.type == MwLLBackendX11) {
		x11opengl_t* o = handle->internal;

		o->glXMakeCurrent(handle->lowlevel->x11.display, None, NULL);
		o->glXDestroyContext(handle->lowlevel->x11.display, o->gl);

		MwDynamicClose(o->lib);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		/* todo */
	}
#endif

	while(w->parent != NULL)
		w = w->parent;

	w->berserk--;

	free(handle->internal);
}

static void mwOpenGLMakeCurrentImpl(MwWidget handle) {
	/* these swap interval functions belonging here actually stink! */

#ifdef USE_GDI
	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		gdiopengl_t* o				= handle->internal;
		BOOL(APIENTRY * swap_interval_ext)(int) = NULL;

		o->wglMakeCurrent(o->dc, o->gl);

		if((swap_interval_ext =
			MwOpenGLGetProcAddress(handle, "wglSwapIntervalEXT")) != NULL) {
			swap_interval_ext(1);
		}
	}
#endif
#ifdef USE_X11
	if(handle->lowlevel->common.type == MwLLBackendX11) {
		x11opengl_t* o					      = handle->internal;
		void (*swap_interval_ext)(Display*, GLXDrawable, int) = NULL;
		void (*swap_interval_mesa)(unsigned int)	      = NULL;
		void (*swap_interval_sgi)(int)			      = NULL;

		o->glXMakeCurrent(handle->lowlevel->x11.display,
				  handle->lowlevel->x11.window, o->gl);

		if((swap_interval_ext =
			MwOpenGLGetProcAddress(handle, "glXSwapIntervalEXT")) != NULL) {
			swap_interval_ext(handle->lowlevel->x11.display,
					  handle->lowlevel->x11.window, 1);
		}

		if((swap_interval_mesa =
			MwOpenGLGetProcAddress(handle, "glXSwapIntervalMESA")) != NULL) {
			swap_interval_mesa(1);
		}

		if((swap_interval_sgi =
			MwOpenGLGetProcAddress(handle, "glXSwapIntervalSGI")) != NULL) {
			swap_interval_sgi(1);
		}
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;

		if(!eglMakeCurrent(o->egl_display, o->egl_surface, o->egl_surface,
				   o->egl_context)) {
			printf("ERROR: eglMakeCurrent, %0X\n", eglGetError());
		}

		MwLLForceRender(handle->lowlevel);
	}
#endif
}

static void mwOpenGLSwapBufferImpl(MwWidget handle) {
#ifdef USE_GDI
	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		gdiopengl_t* o = handle->internal;

		SwapBuffers(o->dc);
	}
#endif
#ifdef USE_X11
	if(handle->lowlevel->common.type == MwLLBackendX11) {
		x11opengl_t* o = handle->internal;

		o->glXSwapBuffers(handle->lowlevel->x11.display,
				  handle->lowlevel->x11.window);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;
		eglSwapInterval(o->egl_display, 0);
		if(!eglSwapBuffers(o->egl_display, o->egl_surface)) {
			printf("ERROR: eglSwapBuffers, %0X\n", eglGetError());
		};
		wl_egl_window_resize((struct wl_egl_window*)o->egl_window_native,
				     handle->lowlevel->wayland.ww,
				     handle->lowlevel->wayland.wh, 0, 0);
	}
#endif
}

static void* mwOpenGLGetProcAddressImpl(MwWidget handle, const char* name) {
#ifdef USE_GDI
	if(handle->lowlevel->common.type == MwLLBackendGDI) {
		gdiopengl_t* o = handle->internal;

		return o->wglGetProcAddress(name);
	}
#endif
#ifdef USE_X11
	if(handle->lowlevel->common.type == MwLLBackendX11) {
		x11opengl_t* o = handle->internal;

		return o->glXGetProcAddress((const GLubyte*)name);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;
		return eglGetProcAddress(name);
	}
#endif
	return NULL;
}

static void func_handler(MwWidget handle, const char* name, void* out,
			 va_list va) {
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

MwClassRec MwOpenGLClassRec = {wcreate,	     /* create */
			       destroy,	     /* destroy */
			       NULL,	     /* draw */
			       NULL,	     /* click */
			       NULL,	     /* parent_resize */
			       NULL,	     /* prop_change */
			       NULL,	     /* mouse_move */
			       NULL,	     /* mouse_up */
			       NULL,	     /* mouse_down */
			       NULL,	     /* key */
			       func_handler, /* execute */
			       NULL,	     /* tick */
			       NULL,	     /* resize */
			       NULL,	     /* children_update */
			       NULL,	     /* children_prop_change */
			       NULL,	     /* clipboard */
			       NULL,	     /* props_change */
			       NULL, NULL, NULL};
MwClass	   MwOpenGLClass    = &MwOpenGLClassRec;
#else
MWDECL MwClass MwOpenGLClass;

MwClass MwOpenGLClass = NULL;
#endif

#endif

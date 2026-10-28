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

#ifdef USE_QUICKDRAW
#include <agl.h>

typedef AGLPixelFormat (*MWaglChoosePixelFormat)(const AGLDevice* gdevs, GLint ndev, const GLint* attribs);
typedef void (*MWaglDestroyPixelFormat)(AGLPixelFormat pix);
typedef AGLContext (*MWaglCreateContext)(AGLPixelFormat pix, AGLContext share);
typedef GLboolean (*MWaglDestroyContext)(AGLContext ctx);
typedef GLboolean (*MWaglCopyContext)(AGLContext src, AGLContext dst, GLuint mask);
typedef GLboolean (*MWaglSetDrawable)(AGLContext ctx, AGLDrawable draw);
typedef GLboolean (*MWaglSetOffScreen)(AGLContext ctx, GLsizei width, GLsizei height, GLsizei rowbytes, GLvoid* baseaddr);
typedef GLboolean (*MWaglSetCurrentContext)(AGLContext ctx);
typedef GLboolean (*MWaglUpdateContext)(AGLContext ctx);
typedef GLboolean (*MWaglEnable)(AGLContext ctx, GLenum pname);
typedef GLboolean (*MWaglSetInteger)(AGLContext ctx, GLenum pname, const GLint* params);
typedef void (*MWaglSwapBuffers)(AGLContext ctx);

typedef void (*MWglFinish)(void);
typedef void (*MWglGetFloatv)(GLenum pname, GLfloat* params);
typedef void (*MWglGetIntegerv)(GLenum pname, GLint* params);
typedef void (*MWglMatrixMode)(GLenum mode);
typedef void (*MWglLoadMatrixf)(const GLfloat* m);

/* AGL can only render into a rectangle inside of the window. While the widget fits, it renders there directly
 * (hardware accelerated if possible). Otherwise a second context renders into memory the size of the widget,
 * and the visible part of it gets copied to the window, like every other widget */
typedef struct quickdrawopengl {
	AGLPixelFormat format;
	AGLContext     gl;

	AGLPixelFormat off_format;
	AGLContext     off_gl;
	int	       offscreen;
	int	       used; /* the application has set up state, which has to survive switching */

	unsigned char* buffer;
	int	       buffer_width;
	int	       buffer_height;
	PixMapHandle   pixmap;

	void* lib;

	/* what the onscreen context was last set up for */
	WindowPtr window;
	long	  window_serial;
	MwRect	  rect;
	Point	  origin;
	int	  window_height;

	RgnHandle clip;
	RgnHandle saved_clip;

	MWaglChoosePixelFormat	aglChoosePixelFormat;
	MWaglDestroyPixelFormat aglDestroyPixelFormat;
	MWaglCreateContext	aglCreateContext;
	MWaglDestroyContext	aglDestroyContext;
	MWaglCopyContext	aglCopyContext;
	MWaglSetDrawable	aglSetDrawable;
	MWaglSetOffScreen	aglSetOffScreen;
	MWaglSetCurrentContext	aglSetCurrentContext;
	MWaglUpdateContext	aglUpdateContext;
	MWaglEnable		aglEnable;
	MWaglSetInteger		aglSetInteger;
	MWaglSwapBuffers	aglSwapBuffers;

	MWglFinish	glFinish;
	MWglGetFloatv	glGetFloatv;
	MWglGetIntegerv glGetIntegerv;
	MWglMatrixMode	glMatrixMode;
	MWglLoadMatrixf glLoadMatrixf;
} quickdrawopengl_t;

#define agl_current(o) ((o)->offscreen ? (o)->off_gl : (o)->gl)

static int agl_resize_buffer(quickdrawopengl_t* o, int width, int height) {
	PixMap*	       pm;
	unsigned char* buffer;

	if(width < 1) width = 1;
	if(height < 1) height = 1;
	if(o->buffer != NULL && o->buffer_width == width && o->buffer_height == height) return 1;

	if((buffer = MwLLQuickdrawAllocBig(width * height * 4)) == NULL) return 0;
	memset(buffer, 0, width * height * 4);

	if(!o->aglSetOffScreen(o->off_gl, width, height, width * 4, buffer)) {
		MwLLQuickdrawFreeBig(buffer);
		return 0;
	}

	MwLLQuickdrawFreeBig(o->buffer);
	o->buffer	 = buffer;
	o->buffer_width	 = width;
	o->buffer_height = height;

	/* 32-bit direct pixmap over the buffer, for CopyBits */
	pm	       = *o->pixmap;
	pm->baseAddr   = (Ptr)buffer;
	pm->rowBytes   = (width * 4) | 0x8000;
	pm->pixelType  = RGBDirect;
	pm->pixelSize  = 32;
	pm->cmpCount   = 3;
	pm->cmpSize    = 8;
	pm->packType   = 0;
	pm->packSize   = 0;
	pm->pmVersion  = 0;
	SetRect(&pm->bounds, 0, 0, width, height);

	return 1;
}

/* carry the state of the application over when switching between the contexts */
static void agl_copy_state(quickdrawopengl_t* o, AGLContext from, AGLContext to) {
	static const GLenum matrices[] = {GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE};
	static const GLenum queries[]  = {GL_MODELVIEW_MATRIX, GL_PROJECTION_MATRIX, GL_TEXTURE_MATRIX};
	GLfloat		    m[3][16];
	GLint		    mode;
	int		    i;

	/* attributes, as glPushAttrib knows them. the matrices are not part of that */
	o->aglCopyContext(from, to, GL_ALL_ATTRIB_BITS);

	o->aglSetCurrentContext(from);
	o->glGetIntegerv(GL_MATRIX_MODE, &mode);
	for(i = 0; i < 3; i++) o->glGetFloatv(queries[i], m[i]);

	o->aglSetCurrentContext(to);
	for(i = 0; i < 3; i++) {
		o->glMatrixMode(matrices[i]);
		o->glLoadMatrixf(m[i]);
	}
	o->glMatrixMode(mode);
}

static void agl_switch(MwWidget handle, quickdrawopengl_t* o, int offscreen) {
	AGLContext from = agl_current(o);

	if(offscreen && o->off_gl == NULL) {
		GLint attribs[] = {AGL_RGBA, AGL_OFFSCREEN, AGL_PIXEL_SIZE, 32, AGL_DEPTH_SIZE, 16, AGL_NONE};

		if((o->off_format = o->aglChoosePixelFormat(NULL, 0, attribs)) == NULL) return;

		/* share display lists and textures, if the renderers allow it */
		if((o->off_gl = o->aglCreateContext(o->off_format, o->gl)) == NULL) o->off_gl = o->aglCreateContext(o->off_format, NULL);
		if(o->off_gl == NULL) {
			o->aglDestroyPixelFormat(o->off_format);
			o->off_format = NULL;
			return;
		}
	}

	if(offscreen) {
		if(!agl_resize_buffer(o, MwGetInteger(handle, MwNwidth), MwGetInteger(handle, MwNheight))) return;

		o->aglSetDrawable(o->gl, NULL);
		o->window = NULL;
	}

	if(o->used) agl_copy_state(o, from, offscreen ? o->off_gl : o->gl);
	o->offscreen = offscreen;
}

/* follow the widget and its window around */
static void agl_sync(MwWidget handle, quickdrawopengl_t* o) {
	MwRect	  r;
	long	  serial;
	WindowPtr w	 = MwLLQuickdrawGetWindow(handle->lowlevel, &r, &serial);
	Point	  origin = {0, 0};
	int	  height = 0;
	int	  force	 = 0;

	if(w != NULL) {
		GrafPtr old;
		int	width = w->portRect.right - w->portRect.left;
		int	fits;

		GetPort(&old);
		SetPort(w);
		LocalToGlobal(&origin);
		SetPort(old);

		height = w->portRect.bottom - w->portRect.top;

		fits = r.x >= 0 && r.y >= 0 && r.x + r.width <= width && r.y + r.height <= height;
		if(fits == o->offscreen) agl_switch(handle, o, !fits);
	}

	if(o->offscreen) {
		if(w != NULL) agl_resize_buffer(o, r.width, r.height);
		return;
	}

	/* the window gets recreated when its style changes, possibly at the same address */
	if(w != o->window || (w != NULL && serial != o->window_serial)) {
		o->aglSetDrawable(o->gl, (AGLDrawable)w);
		o->window	 = w;
		o->window_serial = serial;
		force		 = 1;
	}
	if(w == NULL) return;

	if(force || memcmp(&r, &o->rect, sizeof(r)) != 0 || origin.h != o->origin.h || origin.v != o->origin.v || height != o->window_height) {
		/* the buffer rect is measured from the bottom of the window. it only reaches outside of the window
		 * when there is no offscreen renderer, AGL would fall back to the whole window then */
		int   width = w->portRect.right - w->portRect.left;
		int   x0    = r.x < 0 ? 0 : r.x;
		int   y0    = r.y < 0 ? 0 : r.y;
		int   x1    = r.x + r.width > width ? width : r.x + r.width;
		int   y1    = r.y + r.height > height ? height : r.y + r.height;
		GLint br[4];

		br[0] = x0;
		br[1] = height - y1;
		br[2] = x1 > x0 ? x1 - x0 : 1;
		br[3] = y1 > y0 ? y1 - y0 : 1;

		o->aglSetInteger(o->gl, AGL_BUFFER_RECT, br);
		o->aglEnable(o->gl, AGL_BUFFER_RECT);
		o->aglUpdateContext(o->gl);

		o->rect		 = r;
		o->origin	 = origin;
		o->window_height = height;
	}
}
#endif

#ifdef USE_WAYLAND
#include <EGL/egl.h>
#include <wayland-egl-core.h>
#include <sys/mman.h>
#include <gbm.h>

/*
Note that for Wayland we create a GBM buffer manually and handle blitting it ourselves instead of using wayland-egl. Doing this allows us greater freedom over event handling; wayland-egl constantly pumps events, making us have to add a weird exception to wayland.c to make this work. Doing this also allows us to clip the OpenGL widget, something that was harder (impossible) with egl wayland.
*/
typedef struct waylandopengl {
	EGLDisplay	    egl_display;
	EGLContext	    egl_context;
	EGLSurface	    egl_surface;
	EGLConfig	    egl_config;
	struct gbm_device*  gbm;
	struct gbm_surface* gbm_surface;
	struct gbm_bo*	    previous_bo;
	int		    drm_fd;
	MwPixmap	    frontbuffer;
	MwU8*		    frontbuffer_data;

	void* gllib;
	void* gbmlib;
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
	EGLDisplay (*eglGetPlatformDisplay)(EGLenum platform, void* native_display, const EGLAttrib* attrib_list);
	EGLSurface (*eglCreatePlatformWindowSurface)(EGLDisplay	   display,
						     EGLConfig	   config,
						     void*	   native_window,
						     EGLint const* attrib_list);
	EGLBoolean (*eglChooseConfig)(EGLDisplay    display,
				      EGLint const* attrib_list,
				      EGLConfig*    configs,
				      EGLint	    config_size,
				      EGLint*	    num_config);
	EGLBoolean (*eglGetConfigAttrib)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint* value);
	EGLBoolean (*eglDestroySurface)(EGLDisplay dpy, EGLSurface surface);

	struct gbm_device* (*gbm_create_device)(int fd);
	struct gbm_surface* (*gbm_surface_create)(struct gbm_device* gbm, uint32_t width, uint32_t height, uint32_t format, uint32_t flags);
	void (*gbm_surface_destroy)(struct gbm_surface* surface);

	struct gbm_bo* (*gbm_surface_lock_front_buffer)(struct gbm_surface* surface);
	union gbm_bo_handle (*gbm_bo_get_handle)(struct gbm_bo* bo);
	uint32_t (*gbm_bo_get_stride)(struct gbm_bo* bo);
	uint32_t (*gbm_bo_get_width)(struct gbm_bo* bo);
	uint32_t (*gbm_bo_get_height)(struct gbm_bo* bo);
	void (*gbm_surface_release_buffer)(struct gbm_surface* surface, struct gbm_bo* bo);
	void* (*gbm_bo_map)(struct gbm_bo* bo, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t flags, uint32_t* stride, void** map_data);
	void (*gbm_bo_unmap)(struct gbm_bo* bo, void* map_data);

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
		if(!o->lib) {
			printf("Could not load OpenGL widget under WGL!\n");
			return 1;
		}

#define WGL_FUNC(x) \
	o->x = MwDynamicSymbol(o->lib, #x); \
	if(o->x == NULL) { \
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
#ifdef USE_QUICKDRAW
	if(handle->lowlevel->common.type == MwLLBackendQuickDraw) {
		GLint		   attribs[] = {AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE};
		GLint		   interval  = 1;
		quickdrawopengl_t* o	     = r = malloc(sizeof(*o));

		memset(o, 0, sizeof(*o));

		if((o->lib = MwDynamicOpen("OpenGLLibrary")) == NULL) {
			MwDispatchError(1, "Could not load OpenGL widget under AGL! Is OpenGL installed?");
			free(o);
			return 1;
		}

#define AGL_FUNC(x) \
	if((o->x = (MW##x)MwDynamicSymbol(o->lib, #x)) == NULL) { \
		MwDispatchError(1, "Could not load OpenGL widget under AGL! " #x " missing."); \
		MwDynamicClose(o->lib); \
		free(o); \
		return 1; \
	}

		AGL_FUNC(aglChoosePixelFormat);
		AGL_FUNC(aglDestroyPixelFormat);
		AGL_FUNC(aglCreateContext);
		AGL_FUNC(aglDestroyContext);
		AGL_FUNC(aglCopyContext);
		AGL_FUNC(aglSetDrawable);
		AGL_FUNC(aglSetOffScreen);
		AGL_FUNC(aglSetCurrentContext);
		AGL_FUNC(aglUpdateContext);
		AGL_FUNC(aglEnable);
		AGL_FUNC(aglSetInteger);
		AGL_FUNC(aglSwapBuffers);

		AGL_FUNC(glFinish);
		AGL_FUNC(glGetFloatv);
		AGL_FUNC(glGetIntegerv);
		AGL_FUNC(glMatrixMode);
		AGL_FUNC(glLoadMatrixf);

		if((o->format = o->aglChoosePixelFormat(NULL, 0, attribs)) == NULL || (o->gl = o->aglCreateContext(o->format, NULL)) == NULL) {
			MwDispatchError(1, "Could not load OpenGL widget under AGL! No usable pixel format.");
			if(o->format != NULL) o->aglDestroyPixelFormat(o->format);
			MwDynamicClose(o->lib);
			free(o);
			return 1;
		}

		o->aglSetInteger(o->gl, AGL_SWAP_INTERVAL, &interval);

		o->clip	      = NewRgn();
		o->saved_clip = NewRgn();

		o->pixmap = NewPixMap();
		HLock((Handle)o->pixmap);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		EGLint		 numConfigs;
		EGLint		 numConfigsMatched;
		EGLint		 majorVersion;
		EGLint		 minorVersion;
		EGLContext	 context;
		EGLSurface	 surface;
		EGLint		 fbAttribs[]	  = {EGL_SURFACE_TYPE,
						     EGL_WINDOW_BIT,
						     EGL_RENDERABLE_TYPE,
						     EGL_OPENGL_BIT,
						     EGL_RED_SIZE,
						     8,
						     EGL_GREEN_SIZE,
						     8,
						     EGL_BLUE_SIZE,
						     8,
						     EGL_DEPTH_SIZE,
						     24,
						     EGL_NONE};
		EGLint		 contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION,
						     1,
						     EGL_CONTEXT_MAJOR_VERSION,
						     1,
						     EGL_CONTEXT_MINOR_VERSION,
						     1,
						     EGL_NONE};
		EGLDisplay	 display;
		waylandopengl_t* o = r	    = malloc(sizeof(*o));
		int		 gbm_format = 0;
		EGLConfig	 egl_configs[1024];

		memset(o, 0, sizeof(waylandopengl_t));

		o->gllib = MwDynamicOpen("libEGL.so");
		if(!o->gllib) {
			printf("Could not load OpenGL widget under Wayland! libEGL.so missing.\n");
			return 1;
		}
		o->gbmlib = MwDynamicOpen("libgbm.so");
		if(!o->gbmlib) {
			printf("Could not load OpenGL widget under Wayland! libgbm.so missing.\n");
			return 1;
		}
#define EGL_FUNC(x) \
	o->x = MwDynamicSymbol(o->gllib, #x); \
	if(!o->x) { \
		printf("Could not load OpenGL widget under Wayland! " #x " missing.\n"); \
		return 1; \
	};
#define GBM_FUNC(x) \
	o->x = MwDynamicSymbol(o->gbmlib, #x); \
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
		EGL_FUNC(eglCreatePlatformWindowSurface)
		EGL_FUNC(eglChooseConfig)
		EGL_FUNC(eglGetPlatformDisplay)
		EGL_FUNC(eglGetConfigAttrib)
		EGL_FUNC(eglDestroySurface)
		GBM_FUNC(gbm_create_device)
		GBM_FUNC(gbm_surface_create)
		GBM_FUNC(gbm_surface_destroy)
		GBM_FUNC(gbm_surface_lock_front_buffer)
		GBM_FUNC(gbm_bo_get_handle)
		GBM_FUNC(gbm_bo_get_stride)
		GBM_FUNC(gbm_bo_get_width)
		GBM_FUNC(gbm_bo_get_height)
		GBM_FUNC(gbm_surface_release_buffer)
		GBM_FUNC(gbm_bo_map)
		GBM_FUNC(gbm_bo_unmap)

#define eglGetConfigs o->eglGetConfigs
#define eglInitialize o->eglInitialize
#define eglCreateContext o->eglCreateContext
#define eglMakeCurrent o->eglMakeCurrent
#define eglGetProcAddress o->eglGetProcAddress
#define eglSwapBuffers o->eglSwapBuffers
#define eglSwapInterval o->eglSwapInterval
#define eglGetError o->eglGetError
#define eglBindAPI o->eglBindAPI
#define eglGetDisplay o->eglGetDisplay
#define eglCreatePlatformWindowSurface o->eglCreatePlatformWindowSurface
#define eglDestroySurface o->eglDestroySurface
#define eglChooseConfig o->eglChooseConfig
#define eglGetPlatformDisplay o->eglGetPlatformDisplay

		o->drm_fd = open("/dev/dri/renderD128", O_RDWR);
		assert(o->drm_fd > 0);
		o->gbm = o->gbm_create_device(o->drm_fd);
		assert(o->gbm != NULL);

		display = eglGetPlatformDisplay(0x31D7, o->gbm, NULL);
		if(display == EGL_NO_DISPLAY) {
			printf("ERROR: eglGetDisplay, %0X\n", eglGetError());
			return 1;
		}

		/* Initialize EGL */
		if(!eglInitialize(display, &majorVersion, &minorVersion)) {
			printf("ERROR: eglInitialize, %0X\n", eglGetError());
			return 1;
		}

		eglBindAPI(EGL_OPENGL_API);

		/* Get configs */
		if((eglGetConfigs(display, NULL, 0, &numConfigs) != EGL_TRUE)) {
			printf("ERROR: eglGetConfigs, %0X\n", eglGetError());
			return 1;
		}

		/* Choose config */
		if((eglChooseConfig(display, fbAttribs, egl_configs, numConfigs, &numConfigsMatched) !=
		    EGL_TRUE)) {
			printf("ERROR: eglChooseConfig, %0X\n", eglGetError());
			return 1;
		}

		for(int i = 0; i < numConfigsMatched; i++) {
			if(o->eglGetConfigAttrib(display, egl_configs[i],
						 EGL_NATIVE_VISUAL_ID,
						 &gbm_format) == EGL_FALSE) {
				printf("ERROR: eglGetConfigAttrib, %0X\n", eglGetError());
			} else {
				if(gbm_format == GBM_FORMAT_ARGB8888) {
					o->egl_config = egl_configs[i];
					break;
				}
			}
		}

		/* Create a GL context */
		context = eglCreateContext(display, o->egl_config, EGL_NO_CONTEXT,
					   contextAttribs);
		if(context == EGL_NO_CONTEXT) {
			printf("ERROR: eglCreateContext, %0X\n", eglGetError());
			return 1;
		}

		o->gbm_surface = o->gbm_surface_create(o->gbm, handle->lowlevel->wayland.ww, handle->lowlevel->wayland.wh, GBM_FORMAT_ARGB8888, GBM_BO_USE_LINEAR | GBM_BO_USE_RENDERING);

		/* Create a surface */
		surface = eglCreatePlatformWindowSurface(display, o->egl_config,
							 o->gbm_surface, NULL);
		if(surface == EGL_NO_SURFACE) {
			printf("ERROR: eglCreatePlatformWindowSurface, %0X\n", eglGetError());
			return 1;
		}

		if(!eglMakeCurrent(display, surface, surface, context)) {
			printf("ERROR: eglMakeCurrent (setup): %0X\n", eglGetError());
		}

		o->frontbuffer_data = malloc(handle->lowlevel->wayland.ww * handle->lowlevel->wayland.wh * 4);
		memset(o->frontbuffer_data, 255, handle->lowlevel->wayland.ww * handle->lowlevel->wayland.wh * 4);

		o->frontbuffer = MwLoadRaw(handle, o->frontbuffer_data, handle->lowlevel->wayland.ww, handle->lowlevel->wayland.wh);

		MwVaApply(handle, MwNpixmap, o->frontbuffer, NULL);

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
#ifdef USE_QUICKDRAW
	if(handle->lowlevel->common.type == MwLLBackendQuickDraw) {
		quickdrawopengl_t* o = handle->internal;

		o->aglSetCurrentContext(NULL);
		o->aglSetDrawable(o->gl, NULL);
		o->aglDestroyContext(o->gl);
		o->aglDestroyPixelFormat(o->format);

		if(o->off_gl != NULL) {
			o->aglDestroyContext(o->off_gl);
			o->aglDestroyPixelFormat(o->off_format);
		}
		MwLLQuickdrawFreeBig(o->buffer);

		/* the pixmap does not own the buffer */
		(**o->pixmap).baseAddr = NULL;
		DisposePixMap(o->pixmap);

		DisposeRgn(o->clip);
		DisposeRgn(o->saved_clip);

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
#ifdef USE_QUICKDRAW
	if(handle->lowlevel->common.type == MwLLBackendQuickDraw) {
		quickdrawopengl_t* o = handle->internal;

		agl_sync(handle, o);
		o->aglSetCurrentContext(agl_current(o));
		o->used = 1;
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
#ifdef USE_QUICKDRAW
	if(handle->lowlevel->common.type == MwLLBackendQuickDraw) {
		quickdrawopengl_t* o = handle->internal;

		GrafPtr		   port;
		MwRect		   r;
		WindowPtr	   w;
		long		   serial;

		agl_sync(handle, o);
		if((w = MwLLQuickdrawGetWindow(handle->lowlevel, &r, &serial)) == NULL) return;

		/* the part of the widget that is not covered or scrolled away */
		MwLLQuickdrawGetVisibleRegion(handle->lowlevel, o->clip);

		GetPort(&port);
		SetPort(w);

		if(o->offscreen) {
			Rect src, dst;

			o->aglSetCurrentContext(o->off_gl);
			o->glFinish();

			SetRect(&src, 0, 0, o->buffer_width, o->buffer_height);
			SetRect(&dst, r.x, r.y, r.x + o->buffer_width, r.y + o->buffer_height);

			ForeColor(blackColor);
			BackColor(whiteColor);
			CopyBits((BitMap*)*o->pixmap, &((GrafPtr)w)->portBits, &src, &dst, srcCopy, o->clip);
		} else {
			/* this AGL has no clip region of its own, but the buffer reaches the screen through the window */
			GetClip(o->saved_clip);
			SetClip(o->clip);

			o->aglSwapBuffers(o->gl);

			SetClip(o->saved_clip);
		}

		SetPort(port);
	}
#endif
#ifdef USE_WAYLAND
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;
		struct gbm_bo*	 bo;
		eglSwapInterval(o->egl_display, 0);
		if(!eglSwapBuffers(o->egl_display, o->egl_surface)) {
			printf("ERROR: eglSwapBuffers, %0X\n", eglGetError());
		}
		bo = o->gbm_surface_lock_front_buffer(o->gbm_surface);
		if(bo) {
			void* gbmBoMapData = NULL; // Needed for unmapping if gbm_bo_map is used
			MwU32 width	   = o->gbm_bo_get_width(bo);
			MwU32 height	   = o->gbm_bo_get_height(bo);
			MwU32 stride	   = o->gbm_bo_get_stride(bo);
			void* map	   = o->gbm_bo_map(bo, 0, 0, width, height, GBM_BO_TRANSFER_READ, &stride, &gbmBoMapData);
			MwU32 size	   = height * stride;

			if(map) {
				for(int y = 0; y < height; y++) {
					const MwU8* row = map + y * stride;
					for(int x = 0; x < width; x++) {
						/* Memory layout per pixel: [B][G][R][A] */
						MwU8 b = row[x * 4 + 0];
						MwU8 g = row[x * 4 + 1];
						MwU8 r = row[x * 4 + 2];

						int out			     = (y * width + x) * 4;
						o->frontbuffer_data[out + 0] = r;
						o->frontbuffer_data[out + 1] = g;
						o->frontbuffer_data[out + 2] = b;
						o->frontbuffer_data[out + 3] = 255;
					}
				}

				MwPixmapReloadRaw(
				    o->frontbuffer, o->frontbuffer_data);
			}
			o->gbm_bo_unmap(bo, gbmBoMapData);
		}
		if(o->previous_bo) {
			o->gbm_surface_release_buffer(o->gbm_surface, o->previous_bo);
		}
		o->previous_bo = bo;

		MwForceRender(handle);
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
#ifdef USE_QUICKDRAW
	if(handle->lowlevel->common.type == MwLLBackendQuickDraw) {
		quickdrawopengl_t* o = handle->internal;

		/* OpenGLLibrary exports every gl* function it has */
		return MwDynamicSymbol(o->lib, name);
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

#ifdef USE_WAYLAND
static void frontbuffer_draw(MwWidget handle) {
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;
		MwRect		 r;
		r.x	 = 0;
		r.y	 = 0;
		r.width	 = handle->lowlevel->wayland.ww;
		r.height = handle->lowlevel->wayland.wh;
		MwDrawPixmap(handle, &r, o->frontbuffer);
	}
}
static void frontbuffer_resize(MwWidget handle) {
	if(handle->lowlevel->common.type == MwLLBackendWayland) {
		waylandopengl_t* o = handle->internal;
		if(o->frontbuffer_data) {
			free(o->frontbuffer_data);
		}
		if(o->frontbuffer) {
			MwDestroyPixmap(o->frontbuffer);
		}
		if(o->gbm_surface) {
			o->gbm_surface_destroy(o->gbm_surface);
		}
		if(o->egl_surface) {
			eglDestroySurface(o->egl_display, o->egl_surface);
		}

		o->gbm_surface = o->gbm_surface_create(o->gbm, handle->lowlevel->wayland.ww, handle->lowlevel->wayland.wh, GBM_FORMAT_ARGB8888, GBM_BO_USE_LINEAR | GBM_BO_USE_RENDERING);

		/* Create a surface */
		o->egl_surface = eglCreatePlatformWindowSurface(o->egl_display, o->egl_config,
								o->gbm_surface, NULL);
		if(o->egl_surface == EGL_NO_SURFACE) {
			printf("ERROR: eglCreatePlatformWindowSurface, %0X\n", eglGetError());
		} else {
		}

		if(!eglMakeCurrent(o->egl_display, o->egl_surface, o->egl_surface, o->egl_context)) {
			printf("ERROR: eglMakeCurrent (setup): %0X\n", eglGetError());
		}

		o->frontbuffer_data = malloc(handle->lowlevel->wayland.ww * handle->lowlevel->wayland.wh * 4);
		memset(o->frontbuffer_data, 255, handle->lowlevel->wayland.ww * handle->lowlevel->wayland.wh * 4);

		o->frontbuffer = MwLoadRaw(handle, o->frontbuffer_data, handle->lowlevel->wayland.ww, handle->lowlevel->wayland.wh);

		MwVaApply(handle, MwNpixmap, o->frontbuffer, NULL);
	}
}
#endif

MwClassRec MwOpenGLClassRec = {wcreate, /* create */
			       destroy, /* destroy */
#ifdef USE_WAYLAND
			       frontbuffer_draw, /* draw */
#else
			       NULL, /* draw */
#endif
			       NULL,	     /* click */
			       NULL,	     /* parent_resize */
			       NULL,	     /* prop_change */
			       NULL,	     /* mouse_move */
			       NULL,	     /* mouse_up */
			       NULL,	     /* mouse_down */
			       NULL,	     /* key */
			       func_handler, /* execute */
			       NULL,	     /* tick */
#ifdef USE_WAYLAND
			       frontbuffer_resize, /* resize */
#else
			       NULL, /* resize */
#endif
			       NULL, /* children_update */
			       NULL, /* children_prop_change */
			       NULL, /* clipboard */
			       NULL, /* props_change */
			       NULL, /* drag_and_drop */
			       NULL, /* get_prop */
			       NULL};
MwClass MwOpenGLClass = &MwOpenGLClassRec;
#else
MWDECL MwClass MwOpenGLClass;

MwClass MwOpenGLClass = NULL;
#endif

#endif

#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#include <AvailabilityMacros.h>
#if defined(__LP64__) || (MAC_OS_X_VERSION_MIN_REQUIRED >= 1030)
#include <dlfcn.h>
#define MW_GL_DLSYM
#else
#include <mach-o/dyld.h>
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

typedef struct cocoaopengl {
	NSOpenGLContext* context;
	int		 attached;
	NSRect		 frame;
} cocoaopengl_t;

static int wcreate(MwWidget handle) {
	NSOpenGLPixelFormatAttribute attrs[] = {
	    NSOpenGLPFAColorSize, 24,
	    NSOpenGLPFAAlphaSize, 8,
	    NSOpenGLPFADepthSize, 24,
	    NSOpenGLPFAStencilSize, 8,
	    NSOpenGLPFADoubleBuffer,
	    0};
	NSAutoreleasePool*   pool = [[NSAutoreleasePool alloc] init];
	NSOpenGLPixelFormat* format;
	cocoaopengl_t*	     o = malloc(sizeof(*o));
	MwWidget	     w = handle;

	memset(o, 0, sizeof(*o));

	format	   = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	o->context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
	[format release];

	handle->internal		     = o;
	handle->lowlevel->common.copy_buffer = 0;

	MwSetDefault(handle);

	while(w->parent != NULL) w = w->parent;
	w->berserk++;

	[pool release];

	return 0;
}

static void destroy(MwWidget handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	cocoaopengl_t*	   o	= handle->internal;
	MwWidget	   w	= handle;

	if([NSOpenGLContext currentContext] == o->context) [NSOpenGLContext clearCurrentContext];
	[o->context clearDrawable];
	[o->context release];

	while(w->parent != NULL) w = w->parent;
	w->berserk--;

	free(o);

	[pool release];
}

static void sync_view(MwWidget handle) {
	cocoaopengl_t* o    = handle->internal;
	NSView*	       view = MwLLCocoaGetView(handle->lowlevel);
	NSRect	       f;

	if(o->context == nil || [view window] == nil || ![[view window] isVisible]) return;

	if(!o->attached) {
		[o->context setView:view];
		o->attached = 1;
	}

	f = [view frame];
	if(!NSEqualRects(f, o->frame)) {
		o->frame = f;
		[o->context update];
	}
}

static void mwOpenGLMakeCurrentImpl(MwWidget handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	cocoaopengl_t*	   o	= handle->internal;

	sync_view(handle);
	[o->context makeCurrentContext];

	[pool release];
}

static void mwOpenGLSwapBufferImpl(MwWidget handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	cocoaopengl_t*	   o	= handle->internal;

	sync_view(handle);
	[o->context flushBuffer];

	[pool release];
}

static void* mwOpenGLGetProcAddressImpl(MwWidget handle, const char* name) {
#ifdef MW_GL_DLSYM
	(void)handle;

	return dlsym(RTLD_DEFAULT, name);
#else
	char buf[256];

	(void)handle;

	buf[0] = '_';
	strncpy(buf + 1, name, sizeof(buf) - 2);
	buf[sizeof(buf) - 1] = 0;

	if(!NSIsSymbolNameDefined(buf)) return NULL;
	return NSAddressOfSymbol(NSLookupAndBindSymbol(buf));
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
			       NULL,
			       NULL,
			       NULL,
			       NULL};
MwClass	   MwOpenGLClass    = &MwOpenGLClassRec;

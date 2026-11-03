#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

typedef struct cocoaopengl {
	NSOpenGLContext* context;
	NSView*		 view;
	int		 attached;
	int		 viewport;
	NSRect		 visible;
	NSRect		 placed;
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

	o->view = [[NSView alloc] initWithFrame:NSZeroRect];
	[MwLLCocoaGetView(handle->lowlevel) addSubview:o->view];

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
	[o->view removeFromSuperview];
	[o->view release];

	while(w->parent != NULL) w = w->parent;
	w->berserk--;

	free(o);

	[pool release];
}

static void sync_view(MwWidget handle) {
	cocoaopengl_t* o    = handle->internal;
	NSView*	       view = MwLLCocoaGetView(handle->lowlevel);
	NSRect	       v, p;

	if(o->context == nil || [view window] == nil || ![[view window] isVisible]) {
		return;
	}

	v = [view visibleRect];
	p = [view convertRect:[view bounds] toView:nil];
	if(NSIsEmptyRect(v)) {
		return;
	}

	if(!NSEqualRects(v, o->visible) || !NSEqualRects(p, o->placed)) {
		o->visible  = v;
		o->placed   = p;
		o->viewport = 1;
		[o->view setFrame:v];
		if(o->attached) {
			[o->context update];
		}
	}

	if(!o->attached) {
		[o->context setView:o->view];
		o->attached = 1;
	}
}

static void mwOpenGLMakeCurrentImpl(MwWidget handle) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	cocoaopengl_t*	   o	= handle->internal;

	sync_view(handle);
	[o->context makeCurrentContext];

	if(o->viewport) {
		void (*viewport)(GLint, GLint, GLsizei, GLsizei) = MwDynamicSymbol(NULL, "glViewport");

		o->viewport = 0;
		if(viewport != NULL) {
			viewport(-o->visible.origin.x, -o->visible.origin.y, o->placed.size.width, o->placed.size.height);
		}
	}

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
	(void)handle;

	return MwDynamicSymbol(NULL, name);
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

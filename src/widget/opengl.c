/* $Id$ */
#include "Mw/LowLevel.h"
#include "Mw/TypeDefs.h"
#include <Mw/Milsko.h>
#include <Mw/Widget/OpenGL.h>

#ifdef BUILD_OPENGL

static int create(MwWidget handle) {
	handle->internal		     = MwLLGLCreate(handle->lowlevel);
	handle->lowlevel->common.copy_buffer = 0;
	MwSetDefault(handle);
	return 0;
}

static void destroy(MwWidget handle) {
	MwLLGLDestroy(handle->lowlevel, handle->internal);
}

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwOpenGLMakeCurrent") == 0) {
		MwLLGLMakeCurrent(handle->lowlevel, handle->internal);
	}
	if(strcmp(name, "mwOpenGLSwapBuffer") == 0) {
		MwLLGLSwapBuffer(handle->lowlevel, handle->internal);
	}
	if(strcmp(name, "mwOpenGLGetProcAddress") == 0) {
		const char* _name = va_arg(va, const char*);
		*(void**)out	  = MwLLGLGetProcAddress(handle->lowlevel, handle->internal, _name);
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
#endif

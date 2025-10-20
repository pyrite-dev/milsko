/* $Id$ */
/*!
 * %file Mw/Widget/OpenGL.h
 * %brief OpenGL widget
 */
#ifndef __MW_WIDGET_OPENGL_H__
#define __MW_WIDGET_OPENGL_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/Core.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
#include <GL/gl.h>

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief OpenGL widget class
 */
MWDECL MwClass MwOpenGLClass;

/*!
 * %brief Make a widget current OpenGL context
 * %param handle Widget
 */
MwInline void MwOpenGLMakeCurrent(MwWidget handle) {
	MwWidgetExecute(handle, "mwOpenGLMakeCurrent", NULL);
};

/*!
 * %brief Get a procedure from OpenGL
 * %param handle Widget
 * %param name Name
 * %return Procedure
 */
MwInline void* MwOpenGLGetProcAddress(MwWidget handle, const char* name) {
	void* out;
	MwWidgetExecute(handle, "mwOpenGLGetProcAddress", &out, name);
	return out;
};

/*!
 * %brief Swaps the buffer of OpenGL context
 * %param handle Widget
 */
MwInline void MwOpenGLSwapBuffer(MwWidget handle) {
	MwWidgetExecute(handle, "mwOpenGLSwapBuffer", NULL);
};

#ifdef __cplusplus
}
#endif

#endif

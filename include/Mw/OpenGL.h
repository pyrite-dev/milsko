/* $Id$ */
/*!
 * %file Mw/OpenGL.h
 * %brief OpenGL widget
 */
#ifndef __MW_OPENGL_H__
#define __MW_OPENGL_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

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
MWDECL void MwOpenGLMakeCurrent(MwWidget handle);

/*!
 * %brief Swaps the buffer of OpenGL context
 * %param handle Widget
 */
MWDECL void MwOpenGLSwapBuffer(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif

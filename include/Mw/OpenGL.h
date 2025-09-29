/* $Id$ */
#ifndef __MW_OPENGL_H__
#define __MW_OPENGL_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

MWDECL MwClass MwOpenGLClass;

MWDECL void MwOpenGLMakeCurrent(MwWidget handle);
MWDECL void MwOpenGLSwapBuffer(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif

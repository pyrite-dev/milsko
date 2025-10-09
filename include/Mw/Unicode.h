/* $Id$ */
/*!
 * %file Mw/Unicode.h
 * %brief Handles UTF8 stuff
 */
#ifndef __MW_UNICODE_H__
#define __MW_UNICODE_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Converts UTF-8 to UTF-32
 * %brief input Input
 * %brief output Output
 * %return Bytes this multibyte takes
 */
MWDECL int MwUTF8ToUTF32(const char* input, int* output);

/*!
 * %brief Calculates UTF-8 string length
 * %brief input Input
 * %return Length
 */
MWDECL int MwUTF8Length(const char* input);

#ifdef __cplusplus
}
#endif

#endif

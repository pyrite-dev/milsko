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

/*!
 * %brief Copies UTF-8 string to other string
 * %brief src Source
 * %brief srcskip Length to be skipped
 * %brief dst Destination
 * %brief dstskip Length to be skipped
 * %brief len Length
 * %return Copied length in bytes
 */
MWDECL int MwUTF8Copy(const char* src, int srcskip, char* dst, int dstskip, int len);

#ifdef __cplusplus
}
#endif

#endif

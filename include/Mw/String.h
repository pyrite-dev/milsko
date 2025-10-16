/* $Id$ */
/*!
 * %file Mw/String.h
 * %brief String utilities
 */
#ifndef __MW_STRING_H__
#define __MW_STRING_H__

#include <Mw/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Duplicates a string
 * %param str String
 * %return String
 */
MWDECL char* MwStringDupliacte(const char* str);

/*!
 * %brief Concatenates 2 strings
 * %param str1 String
 * %param str2 String
 * %return String
 */
MWDECL char* MwStringConcat(const char* str1, const char* str2);

#ifdef __cplusplus
}
#endif

#endif

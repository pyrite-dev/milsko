/* $Id$ */
/*!
 * %file Mw/Font.h
 * %brief Default font
 */
#ifndef __MW_FONT_H__
#define __MW_FONT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Default font data
 */
MWDECL MwFont MwFontData[];

/*!
 * %brief Default bold font data
 */
MWDECL MwFont MwBoldFontData[];

/*!
 * %brief Default TTF font data
 */
MWDECL unsigned char MwTTFData[];

/*!
 * %brief Default TTF font size
 */
MWDECL unsigned int MwTTFDataSize;

/*!
 * %brief Default bold TTF font data
 */
MWDECL unsigned char MwBoldTTFData[];

/*!
 * %brief Default bold TTF font size
 */
MWDECL unsigned int MwBoldTTFDataSize;

/*!
 * %brief Loads a TTF Font
 * %param data Data
 * %param size Data size
 * %return Font handle
 */
MWDECL void* MwFontLoad(unsigned char* data, unsigned int size);

/*!
 * %brief Frees a font handle
 * %param handle Handle
 */
MWDECL void MwFontFree(void* handle);

#ifdef __cplusplus
}
#endif

#endif

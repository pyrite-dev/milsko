/* $Id$ */
/*!
 * @file Mw/Resource/Font.h
 * @brief Default font resources
 */
#ifndef __MW_RESOURCE__FONT_H__
#define __MW_RESOURCE_FONT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Default font data
 */
MWDECL MwFont MwFontData[];

/*!
 * @brief Default bold font data
 */
MWDECL MwFont MwBoldFontData[];

/*!
 * @brief Default TTF font data
 */
MWDECL unsigned char MwTTFData[];

/*!
 * @brief Default TTF font size
 */
MWDECL unsigned int MwTTFDataSize;

/*!
 * @brief Default bold TTF font data
 */
MWDECL unsigned char MwBoldTTFData[];

/*!
 * @brief Default bold TTF font size
 */
MWDECL unsigned int MwBoldTTFDataSize;

#ifdef __cplusplus
}
#endif

#endif

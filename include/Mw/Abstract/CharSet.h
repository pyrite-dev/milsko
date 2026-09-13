/*!
 * @file Mw/Abstract/CharSet.h
 * @brief CharSet
 */
#ifndef __MW_ABSTRACT_CHARSET_H__
#define __MW_ABSTRACT_CHARSET_H__

#include <Mw/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

MWDECL char* MWAPI MwACPTextToUTF8Text(const char* input);

MWDECL char* MWAPI MwUTF8TextToACPText(const char* input);

#ifdef __cplusplus
}
#endif

#endif

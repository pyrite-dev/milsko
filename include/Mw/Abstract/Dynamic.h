/* $Id$ */
/*!
 * @file Mw/Abstract/Dynamic.h
 * @brief Dynamic library
 */
#ifndef __MW_ABSTRACT_DYNAMIC_H__
#define __MW_ABSTRACT_DYNAMIC_H__

#include <Mw/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Opens a dynamic library
 * @param path Path
 * @return Handle
 */
MWDECL void* MwDynamicOpen(const char* path);

/*!
 * @brief Gets a symbol from the dynamic library
 * @param handle Handle
 * @param symbol Symbol
 */
MWDECL void* MwDynamicSymbol(void* handle, const char* symbol);

/*!
 * @brief Closes a dynamic library
 * @param handle Handle
 */
MWDECL void MwDynamicClose(void* handle);

#ifdef __cplusplus
}
#endif

#endif

/* $Id$ */
/*!
 * %file Mw/Directory.h
 * %brief Directory functions
 */
#ifndef __MW_DIRECTORY_H__
#define __MW_DIRECTORY_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Opens a directory
 * %param path Path
 * %return Handle
 */
MWDECL void* MwDirectoryOpen(const char* path);

/*!
 * %brief Closes a directory
 * %param handle Handle
 */
MWDECL void MwDirectoryClose(void* handle);

/*!
 * %brief Reads a directory
 * %param handle Handle
 * %return Directory entry
 */
MWDECL MwDirectoryEntry* MwDirectoryRead(void* handle);

/*!
 * %brief Frees a directory entry
 * %param entry Entry
 */
MWDECL void MwDirectoryFreeEntry(MwDirectoryEntry* entry);

#ifdef __cplusplus
}
#endif

#endif

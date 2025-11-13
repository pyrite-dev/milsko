/* $Id$ */
/*!
 * @file Mw/Abstract/Mutex.h
 * @brief Mutex
 */
#ifndef __MW_ABSTRACT_MUTEX_H__
#define __MW_ABSTRACT_MUTEX_H__

#include <Mw/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Creates a mutex
 * @return Handle
 */
MWDECL void* MwMutexCreate(void);

/*!
 * @brief Destroys a mutex
 * @param handle Handle
 */
MWDECL void MwMutexDestroy(void* handle);

/*!
 * @brief Locks a mutex
 * @param handle Handle
 */
MWDECL void MwMutexLock(void* handle);

/*!
 * @brief Unlocks a mutex
 * @param handle Handle
 */
MWDECL void MwMutexUnlock(void* handle);

#ifdef __cplusplus
}
#endif

#endif

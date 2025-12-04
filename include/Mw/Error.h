/*!
 * @file Mw/Error.h
 * @brief Error codes
 */
#ifndef __MW_ERROR_H__
#define __MW_ERROR_H__

#include <Mw/MachDep.h>

/*!
 * @brief Error code enumeration
 */
typedef enum MwErrorEnum_T {
	/*!
	 * @brief No error
	 */
	MwEsuccess = 0,
	/*!
	 * @brief There was an error
	 */
	MwEerror,
} MwErrorEnum;

/*!
 * @brief Get the last error
 */
MWDECL const char* MwGetLastError(void);

#endif

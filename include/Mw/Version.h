/*!
 * @file Mw/Version.h
 * @brief Version informations
 */
#ifndef __MW_VERSION_H__
#define __MW_VERSION_H__

/*!
 * @brief Major version
 */
#define MwMAJOR 1

/*!
 * @brief Minor version
 */
#define MwMINOR 1

/*!
 * @brief Patchlevel of version, calculated by `patchlevel_alphabet - 'a'`
 */
#define MwPATCH 0

/*!
 * @brief Version in string
 */
#define MwVERSION "1.1"

/*!
 * @brief Version in string
 */
MWDECL const char* MwVersionString;

#endif

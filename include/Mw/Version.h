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
#define MwMINOR 2

/*!
 * @brief Patchlevel of version, calculated by `patchlevel_alphabet - 'a' + 1`
 */
#define MwPATCH 1

/*!
 * @brief Version in string
 */
#define MwVERSION "1.2a"

/*!
 * @brief Version in string
 */
MWDECL const char* MwVersionString;

#endif

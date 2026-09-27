/*!
 * @file Mw/Version.h
 * @brief Version informations
 */
#ifndef __MW_VERSION_H__
#define __MW_VERSION_H__

/*!
 * @brief Major version
 */
#define MwMAJOR 2

/*!
 * @brief Minor version
 */
#define MwMINOR 0

/*!
 * @brief Patchlevel of version, if patchlevel_alphabet is a or bigger, this value is calculated by `patchlevel_alphabet - 'a' + 1`. Otherwise 0
 */
#define MwPATCH 1

/*!
 * @brief Version in string
 */
#define MwVERSION "2.0a"

/*!
 * @brief Version in string
 */
MWDECL const char* MwVersionString;

#endif

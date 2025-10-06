/* $Id$ */
/*!
 * %file Mw/Cursor.h
 * %brief Cursor externs
 */

#ifndef __MW_CURSOR_H__
#define __MW_CURSOR_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Default cursor
 */
MWDECL MwCursor MwCursorDefault;

/*!
 * %brief Default cursor mask
 */
MWDECL MwCursor MwCursorDefaultMask;

/*!
 * %brief Cross cursor
 */
MWDECL MwCursor MwCursorCross;

/*!
 * %brief Cross cursor mask
 */
MWDECL MwCursor MwCursorCrossMask;

#ifdef __cplusplus
}
#endif

#endif

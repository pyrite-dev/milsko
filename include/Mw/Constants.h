/* $Id$ */
/*!
 * %file Mw/Constants.h
 * %brief Constants
 */
#ifndef __MW_CONSTANTS_H__
#define __MW_CONSTANTS_H__

/*!
 * %brief Direction enumeration
 */
enum MwDIRECTION {
	MwNORTH = 0,
	MwSOUTH,
	MwEAST,
	MwWEST
};

/*!
 * %brief Orientation
 */
enum MwORIENTATION {
	MwVERTICAL = 0,
	MwHORIZONTAL
};

/*!
 * %brief Alignment
 */
enum MwALIGNMENT {
	MwALIGNMENT_CENTER = 0,
	MwALIGNMENT_BEGINNING,
	MwALIGNMENT_END,
};

/*!
 * %brief Default
 */
#define MwDEFAULT 0x0fffffff

/*!
 * %brief Icon mask
 */
#define MwMB_ICONMASK 0xf

/*!
 * %brief Warning icon
 */
#define MwMB_ICONWARNING 0x1

/*!
 * %brief Information icon
 */
#define MwMB_ICONINFO 0x2

/*!
 * %brief Note icon
 */
#define MwMB_ICONNOTE 0x3

/*!
 * %brief Question icon
 */
#define MwMB_ICONQUESTION 0x4

/*!
 * %brief News icon
 */
#define MwMB_ICONNEWS 0x5

#endif

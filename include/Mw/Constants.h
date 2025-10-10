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

#endif

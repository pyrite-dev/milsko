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
	MwALIGNMENT_END
};

/*!
 * %brief Default
 */
#define MwDEFAULT 0x0fffffff

/*!
 * %brief Directory entry type
 */
enum MwDIRECTORY_TYPE {
	MwDIRECTORY_FILE = 0,
	MwDIRECTORY_DIRECTORY
};

/*!
 * %brief Icon mask
 */
#define MwMB_ICONMASK 0xf

enum MwMB_ICON {
	MwMB_ICONWARNING = 0x1,
	MwMB_ICONINFO,
	MwMB_ICONNOTE,
	MwMB_ICONNEWS,
	MwMB_ICONERROR
};

/*!
 * %brief Button mask
 */
#define MwMB_BUTTONMASK 0xf0

/*!
 * %brief OK button
 */
#define MwMB_BUTTONOK 0x10

/*!
 * %brief Cancel button
 */
#define MwMB_BUTTONCANCEL 0x20

/*!
 * %brief Yes button
 */
#define MwMB_BUTTONYES 0x40

/*!
 * %brief No button
 */
#define MwMB_BUTTONNO 0x80

/*!
 * %brief Ok and Cancel button
 */
#define MwMB_BUTTONOKCANCEL (MwMB_BUTTONCANCEL | MwMB_BUTTONOK)

/*!
 * %brief Yes and No button
 */
#define MwMB_BUTTONYESNO (MwMB_BUTTONYES | MwMB_BUTTONNO)

/*!
 * %brief Yes, No and Cancel button
 */
#define MwMB_BUTTONYESNOCANCEL (MwMB_BUTTONYES | MwMB_BUTTONNO | MwMB_BUTTONCANCEL)

#endif

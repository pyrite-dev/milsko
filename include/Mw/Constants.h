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

/*!
 * %brief Error icon
 */
#define MwMB_ICONERROR 0x6

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

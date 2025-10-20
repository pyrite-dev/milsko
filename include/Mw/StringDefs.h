/* $Id$ */
/*!
 * %file Mw/StringDefs.h
 * %brief String definitions for property
 */
#ifndef __MW_STRINGDEFS_H__
#define __MW_STRINGDEFS_H__

#define MwNx "Ix"
#define MwNy "Iy"
#define MwNwidth "Iwidth"
#define MwNheight "Iheight"
#define MwNorientation "Iorientation"
#define MwNminValue "IminValue"
#define MwNmaxValue "ImaxValue"
#define MwNvalue "Ivalue"
#define MwNchangedBy "IchangedBy"
#define MwNareaShown "IareaShown"
#define MwNchecked "Ichecked"
#define MwNalignment "Ialignment"
#define MwNbold "Ibold"
#define MwNmain "Imain"
#define MwNleftPadding "IleftPadding"
#define MwNhasHeading "IhasHeading"
#define MwnhasBorder "IhasBorder"
#define MwNinverted "Iinverted"

#define MwNtitle "Stitle"
#define MwNtext "Stext"
#define MwNbackground "Sbackground"
#define MwNforeground "Sforeground"

#define MwNpixmap "Vpixmap"
#define MwNiconPixmap "ViconPixmap"
#define MwNsizeHints "VsizeHints"

#define MwNactivateHandler "Cactivate"	   /* NULL/int* (MwListBox) */
#define MwNresizeHandler "Cresize"	   /* NULL */
#define MwNtickHandler "Ctick"		   /* NULL */
#define MwNmenuHandler "Cmenu"		   /* MwMenu */
#define MwNmouseDownHandler "CmouseDown"   /* MwLLMouse* */
#define MwNmouseUpHandler "CmouseUp"	   /* asme as MwNmouseDownHandler */
#define MwNmouseMoveHandler "CmouseMove"   /* MwPoint* */
#define MwNchangedHandler "Cchanged"	   /* NULL */
#define MwNkeyHandler "Ckey"		   /* int* (MwLLKeyEnum or character code) */
#define MwNkeyReleaseHandler "CkeyRelease" /* same as MwNkeyHandler */
#define MwNcloseHandler "Cclose"	   /* NULL */
#define MwNfocusInHandler "CfocusIn"	   /* NULL */
#define MwNfocusOutHandler "CfocusOut"	   /* NULL */
#define MwNfileChosenHandler "CfileChosen"	   /* char* */

#endif

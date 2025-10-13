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
#define MwNareaShown "IareaShown"
#define MwNchecked "Ichecked"
#define MwNalignment "Ialignment"
#define MwNbold "Ibold"
#define MwNmain "Imain"

#define MwNtitle "Stitle"
#define MwNtext "Stext"
#define MwNbackground "Sbackground"
#define MwNforeground "Sforeground"

#define MwNpixmap "Vpixmap"
#define MwNiconPixmap "ViconPixmap"
#define MwNsizeHints "VsizeHints"

#define MwNactivateHandler "Cactivate"		  /* NULL */
#define MwNresizeHandler "Cresize"		  /* NULL */
#define MwNtickHandler "Ctick"			  /* NULL */
#define MwNmenuHandler "Cmenu"			  /* MwMenu */
#define MwNmouseDownHandler "CmouseDownHandler"	  /* MwLLMouse* */
#define MwNmouseUpHandler "CmouseUpHandler"	  /* asme as MwNmouseDownHandler */
#define MwNmouseMoveHandler "CmouseMoveHandler"	  /* MwPoint* */
#define MwNchangedHandler "CchangedHandler"	  /* NULL */
#define MwNkeyHandler "CkeyHandler"		  /* int* (MwLLKeyEnum or character code) */
#define MwNkeyReleaseHandler "CkeyReleaseHandler" /* same as MwNkeyHandler */
#define MwNcloseHandler "CcloseHandler"		  /* NULL */

#endif

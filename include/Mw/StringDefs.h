/*!
 * @file Mw/StringDefs.h
 * @brief String definitions for property
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
#define MwNhasBorder "IhasBorder"
#define MwNinverted "Iinverted"
#define MwNmodernLook "ImodernLook"
#define MwNwaitMS "IwaitMS"
#define MwNhideInput "IhideInput"
#define MwNsingleClickSelectable "IsingleClickSelectable"
#define MwNflat "Iflat"
#define MwNshowArrows "IshowArrows"
#define MwNpadding "Ipadding"
#define MwNborderWidth "IborderWidth"
#define MwNfillArea "IfillArea"
#define MwNratio "Iratio"
#define MwNfixedSize "IfixedSize"
#define MwNouterPadding "IouterPadding"

#define MwNtitle "Stitle"
#define MwNtext "Stext"
#define MwNbackground "Sbackground"
#define MwNsubBackground "SsubBackground"
#define MwNforeground "Sforeground"
#define MwNsubForeground "SsubForeground"

#define MwNpixmap "Vpixmap"
#define MwNiconPixmap "ViconPixmap"
#define MwNsizeHints "VsizeHints"
#define MwNfont "Vfont"
#define MwNboldFont "VboldFont"
#define MwNbackgroundPixmap "VbackgroundPixmap"

#define MwNactivateHandler "Cactivate"		     /* NULL/int* (MwListBox)/void* (MwTreeView) */
#define MwNresizeHandler "Cresize"		     /* NULL */
#define MwNtickHandler "Ctick"			     /* NULL */
#define MwNmenuHandler "Cmenu"			     /* MwMenu */
#define MwNmouseDownHandler "CmouseDown"	     /* MwLLMouse* */
#define MwNmouseUpHandler "CmouseUp"		     /* same as MwNmouseDownHandler */
#define MwNmouseMoveHandler "CmouseMove"	     /* MwPoint* */
#define MwNchangedHandler "Cchanged"		     /* NULL/int* (MwComboBox) */
#define MwNkeyHandler "Ckey"			     /* int* (MwLLKeyEnum or character code) */
#define MwNkeyReleaseHandler "CkeyRelease"	     /* same as MwNkeyHandler */
#define MwNcloseHandler "Cclose"		     /* NULL */
#define MwNfocusInHandler "CfocusIn"		     /* NULL */
#define MwNfocusOutHandler "CfocusOut"		     /* NULL */
#define MwNfileChosenHandler "CfileChosen"	     /* char* */
#define MwNdirectoryChosenHandler "CdirectoryChosen" /* char* */
#define MwNcolorChosenHandler "CcolorChosen"	     /* MwRGB* */
#define MwNdrawHandler "Cdraw"			     /* NULL */

#endif

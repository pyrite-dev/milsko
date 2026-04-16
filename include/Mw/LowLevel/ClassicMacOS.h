/*!
 * @file Mw/LowLevel/ClassicMacOS.h
 * @brief ClassicMacOS Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_ClassicMacOS_H__
#define __MW_LOWLEVEL_ClassicMacOS_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

struct _MwLLClassicMacOS {
	struct _MwLLCommon common;

	WindowRef   window;
	EventRecord event;
	Rect	    winRect;
};

struct _MwLLClassicMacOSColor {
	struct _MwLLCommonColor common;
};

struct _MwLLClassicMacOSPixmap {
	struct _MwLLCommonPixmap common;
};

MWDECL int MwLLClassicMacOSCallInit(void);

#endif

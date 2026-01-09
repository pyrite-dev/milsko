/*!
 * @file Mw/LowLevel/AppKit.h
 * @brief Work in progress AppKit Backend
 * @warning This is used internally.
 */
#ifndef __MW_LOWLEVEL_APPKIT_H__
#define __MW_LOWLEVEL_APPKIT_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

MWDECL int MwLLAppKitCallInit(void);

struct _MwLLAppKit {
	struct _MwLLCommon common;
};

struct _MwLLAppKitColor {
	struct _MwLLCommonColor common;
};

struct _MwLLAppKitPixmap {
	struct _MwLLCommonPixmap common;
};

#endif

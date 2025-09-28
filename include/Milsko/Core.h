/* $Id$ */
#ifndef __MILSKO_CORE_H__
#define __MILSKO_CORE_H__

#ifdef _MILSKO
#include <Milsko/LowLevel.h>

typedef struct _Milsko {
	HMILSKOLL lowlevel;
}* HMILSKO;
#else
typedef void* HMILSKO;
#endif

HMILSKO MilskoCreateWidget(HMILSKO parent, int x, int y, unsigned int width, unsigned int height);
void	MilskoDestroyWidget(HMILSKO handle);

#endif

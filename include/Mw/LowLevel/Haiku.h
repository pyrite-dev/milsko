/*!
 * @file Mw/LowLevel/Haiku.h
 * @brief Haiku Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_HAIKU_H__
#define __MW_LOWLEVEL_HAIKU_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class MwApplication : public BApplication {
	private:
		void ReadyToRun();

	public:
		BWindow* window;
		int ready;
		BLocker* locker;

		MwApplication(BRect rc, MwLL handle);
		~MwApplication();

		void Pulse();
};

class MwView : public BView {
	public:
		MwView(MwApplication* app, BRect rc, uint32 resizing, uint32 flags);
};
#else
typedef void MwApplication;
typedef void MwView;
#endif

struct _MwLLHaiku {
	struct _MwLLCommon common;

	MwLL parent;
	MwApplication* app;
	MwView* view;
};

struct _MwLLHaikuColor {
	struct _MwLLCommonColor common;
};

struct _MwLLHaikuPixmap {
	struct _MwLLCommonPixmap common;
};

MWDECL int     MwLLHaikuCallInit(void);

#ifdef __cplusplus
}
#endif

#endif

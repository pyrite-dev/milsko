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
    public:
	BWindow* window;
	BLocker* locker;

	MwApplication(BRect rc, MwLL handle);
	~MwApplication();

	void MessageReceived(BMessage* message);
};

class MwView : public BView {
    public:
	MwView(BRect rc, uint32 resizing, uint32 flags);
};
#else
typedef void MwApplication;
typedef void MwView;
#endif

enum B_MW_WHAT {
	B_MW_DESTROY = 0
};

struct _MwLLHaiku {
	struct _MwLLCommon common;

	MwLL	       parent;
	MwApplication* app;
	MwView*	       view;
	thread_id      app_thread;
};

struct _MwLLHaikuColor {
	struct _MwLLCommonColor common;
};

struct _MwLLHaikuPixmap {
	struct _MwLLCommonPixmap common;
};

MWDECL int MwLLHaikuCallInit(void);

#ifdef __cplusplus
}
#endif

#endif

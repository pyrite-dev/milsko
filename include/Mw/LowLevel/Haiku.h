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
class MwApplication;
class MwView;
class MwWindow;

class MwApplication : public BApplication {
    public:
	MwWindow* window;

	MwApplication(BRect rc, MwLL handle);
	~MwApplication();

	void MessageReceived(BMessage* message);
};

class MwView : public BView {
    public:
	MwLL	 handle;
	BLocker* locker;

	MwView(MwLL handle, BRect rc, uint32 resizingMode, uint32 flags);
	~MwView();

	void MessageReceived(BMessage* message);
	void PostMessage(BMessage* message);

	void Draw(BRect updateRect);

	void SetColor(MwLLColor color);
};

class MwWindow : public BWindow {
    public:
	MwWindow(BRect rc, window_type type, uint32 flags);

	void MessageReceived(BMessage* message);
};
#else
typedef void MwApplication;
typedef void MwView;
#endif

typedef union _MwLLHaikuEvent MwLLHaikuEvent;

enum BAPP_MW_WHAT {
	BAPP_MW_DESTROY = 0
};

enum BVIEW_MW_WHAT {
	BVIEW_MW_RESIZE = 0,
	BVIEW_MW_MOVE,
	BVIEW_MW_SET_COLOR,
	BVIEW_MW_POLYGON,
	BVIEW_MW_BITMAP
};

enum BWIN_MW_WHAT {
	BWIN_MW_SET_TITLE = 0
};

enum MwLLHAIKU_EVENT {
	MwLLHAIKU_EVENT_DRAW = 0
};

union _MwLLHaikuEvent {
	int type;
};

struct _MwLLHaiku {
	struct _MwLLCommon common;

	int x;
	int y;
	int width;
	int height;

	MwLL	       parent;
	MwApplication* app;
	MwView*	       view;
	thread_id      app_thread;

	MwLLHaikuEvent* events;
};

struct _MwLLHaikuColor {
	struct _MwLLCommonColor common;
};

struct _MwLLHaikuPixmap {
	struct _MwLLCommonPixmap common;

	BBitmap* bitmap;
};

MWDECL int MwLLHaikuCallInit(void);

#ifdef __cplusplus
}
#endif

#endif

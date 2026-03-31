#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

#define ClipboardTimeout 100

static struct {
	void* lib_xlib;
	void* lib_xrender;

	int* _Xdebug;

	XFontStruct* (*XLoadQueryFont)(Display*, _Xconst char*);
	XFontStruct* (*XQueryFont)(Display*, XID);
	XTimeCoord* (*XGetMotionEvents)(Display*, Window, Time, Time, int*);
	XModifierKeymap* (*XDeleteModifiermapEntry)(XModifierKeymap*,
#if NeedWidePrototypes
						    unsigned int,
#else
						    KeyCode,
#endif
						    int);
	XModifierKeymap* (*XGetModifierMapping)(Display*);
	XModifierKeymap* (*XInsertModifiermapEntry)(XModifierKeymap*,
#if NeedWidePrototypes
						    unsigned int,
#else
						    KeyCode,
#endif
						    int);
	XModifierKeymap* (*XNewModifiermap)(int);
	XImage* (*XCreateImage)(Display*, Visual*, unsigned int, int, int, char*, unsigned int, unsigned int, int, int);
	Status (*XInitImage)(XImage*);
	XImage* (*XGetImage)(Display*, Drawable, int, int, unsigned int, unsigned int, unsigned long, int);
	XImage* (*XGetSubImage)(Display*, Drawable, int, int, unsigned int, unsigned int, unsigned long, int, XImage*, int, int); /*	 * X function declarations.	 */
	Display* (*XOpenDisplay)(_Xconst char*);
	void (*XrmInitialize)(void);
	char* (*XFetchBytes)(Display*, int*);
	char* (*XFetchBuffer)(Display*, int*, int);
	char* (*XGetAtomName)(Display*, Atom);
	Status (*XGetAtomNames)(Display*, Atom*, int, char**);
	char* (*XGetDefault)(Display*, _Xconst char*, _Xconst char*);
	char* (*XDisplayName)(_Xconst char*);
	char* (*XKeysymToString)(KeySym);
	// int (*XSynchronize(Display*, Bool))(Display*);
	// int (*XSetAfterFunction(Display*, int (*)(Display*)))(Display*);
	Atom (*XInternAtom)(Display*, _Xconst char*, Bool);
	Status (*XInternAtoms)(Display*, char**, int, Bool, Atom*);
	Colormap (*XCopyColormapAndFree)(Display*, Colormap);
	Colormap (*XCreateColormap)(Display*, Window, Visual*, int);
	Cursor (*XCreatePixmapCursor)(Display*, Pixmap, Pixmap, XColor*, XColor*, unsigned int, unsigned int);
	Cursor (*XCreateGlyphCursor)(Display*, Font, Font, unsigned int, unsigned int, XColor _Xconst*, XColor _Xconst*);
	Cursor (*XCreateFontCursor)(Display*, unsigned int);
	Font (*XLoadFont)(Display*, _Xconst char*);
	GC (*XCreateGC)(Display*, Drawable, unsigned long, XGCValues*);
	GContext (*XGContextFromGC)(GC);
	void (*XFlushGC)(Display*, GC);
	Pixmap (*XCreatePixmap)(Display*, Drawable, unsigned int, unsigned int, unsigned int);
	Pixmap (*XCreateBitmapFromData)(Display*, Drawable, _Xconst char*, unsigned int, unsigned int);
	Pixmap (*XCreatePixmapFromBitmapData)(Display*, Drawable, char*, unsigned int, unsigned int, unsigned long, unsigned long, unsigned int);
	Window (*XCreateSimpleWindow)(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long);
	Window (*XGetSelectionOwner)(Display*, Atom);
	Window (*XCreateWindow)(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);
	Colormap* (*XListInstalledColormaps)(Display*, Window, int*);
	char** (*XListFonts)(Display*, _Xconst char*, int, int*);
	char** (*XListFontsWithInfo)(Display*, _Xconst char*, int, int*, XFontStruct**);
	char** (*XGetFontPath)(Display*, int*);
	char** (*XListExtensions)(Display*, int*);
	Atom* (*XListProperties)(Display*, Window, int*);
	XHostAddress* (*XListHosts)(Display*, int*, Bool*);
	_X_DEPRECATED KeySym (*XKeycodeToKeysym)(Display*,
#if NeedWidePrototypes
						 unsigned int,
#else
						 KeyCode,
#endif
						 int);
	KeySym (*XLookupKeysym)(XKeyEvent*, int);
	KeySym* (*XGetKeyboardMapping)(Display*,
#if NeedWidePrototypes
				       unsigned int,
#else
				       KeyCode,
#endif
				       int, int*);
	KeySym (*XStringToKeysym)(_Xconst char*);
	long (*XMaxRequestSize)(Display*);
	long (*XExtendedMaxRequestSize)(Display*);
	char* (*XResourceManagerString)(Display*);
	char* (*XScreenResourceString)(Screen*);
	unsigned long (*XDisplayMotionBufferSize)(Display*);
	VisualID (*XVisualIDFromVisual)(Visual*);
	Status (*XInitThreads)(void);
	Status (*XFreeThreads)(void);
	void (*XLockDisplay)(Display*);
	void (*XUnlockDisplay)(Display*);
	XExtCodes* (*XInitExtension)(Display*, _Xconst char*);
	XExtCodes* (*XAddExtension)(Display*);
	XExtData* (*XFindOnExtensionList)(XExtData**, int);
	XExtData** (*XEHeadOfExtensionList)(XEDataObject);
	Window (*XRootWindow)(Display*, int);
	Window (*XDefaultRootWindow)(Display*);
	Window (*XRootWindowOfScreen)(Screen*);
	Visual* (*XDefaultVisual)(Display*, int);
	Visual* (*XDefaultVisualOfScreen)(Screen*);
	GC (*XDefaultGC)(Display*, int);
	GC (*XDefaultGCOfScreen)(Screen*);
	unsigned long (*XBlackPixel)(Display*, int);
	unsigned long (*XWhitePixel)(Display*, int);
	unsigned long (*XAllPlanes)(void);
	unsigned long (*XBlackPixelOfScreen)(Screen*);
	unsigned long (*XWhitePixelOfScreen)(Screen*);
	unsigned long (*XNextRequest)(Display*);
	unsigned long (*XLastKnownRequestProcessed)(Display*);
	char* (*XServerVendor)(Display*);
	char* (*XDisplayString)(Display*);
	Colormap (*XDefaultColormap)(Display*, int);
	Colormap (*XDefaultColormapOfScreen)(Screen*);
	Display* (*XDisplayOfScreen)(Screen*);
	Screen* (*XScreenOfDisplay)(Display*, int);
	Screen* (*XDefaultScreenOfDisplay)(Display*);
	long (*XEventMaskOfScreen)(Screen*);
	int (*XScreenNumberOfScreen)(Screen*);
	XErrorHandler (*XSetErrorHandler)(XErrorHandler);
	XIOErrorHandler (*XSetIOErrorHandler)(XIOErrorHandler);
	void (*XSetIOErrorExitHandler)(Display*, XIOErrorExitHandler, void*);
	XPixmapFormatValues* (*XListPixmapFormats)(Display*, int*);
	int* (*XListDepths)(Display*, int, int*);
	Status (*XReconfigureWMWindow)(Display*, Window, int, unsigned int, XWindowChanges*);
	Status (*XGetWMProtocols)(Display*, Window, Atom**, int*);
	Status (*XSetWMProtocols)(Display*, Window, Atom*, int);
	Status (*XIconifyWindow)(Display*, Window, int);
	Status (*XWithdrawWindow)(Display*, Window, int);
	Status (*XGetCommand)(Display*, Window, char***, int*);
	Status (*XGetWMColormapWindows)(Display*, Window, Window**, int*);
	Status (*XSetWMColormapWindows)(Display*, Window, Window*, int);
	void (*XFreeStringList)(char**);
	int (*XSetTransientForHint)(Display*, Window, Window);
	int (*XActivateScreenSaver)(Display*);
	int (*XAddHost)(Display*, XHostAddress*);
	int (*XAddHosts)(Display*, XHostAddress*, int);
	int (*XAddToExtensionList)(struct _XExtData**, XExtData*);
	int (*XAddToSaveSet)(Display*, Window);
	Status (*XAllocColor)(Display*, Colormap, XColor*);
	Status (*XAllocColorCells)(Display*, Colormap, Bool, unsigned long*, unsigned int, unsigned long*, unsigned int);
	Status (*XAllocColorPlanes)(Display*, Colormap, Bool, unsigned long*, int, int, int, int, unsigned long*, unsigned long*, unsigned long*);
	Status (*XAllocNamedColor)(Display*, Colormap, _Xconst char*, XColor*, XColor*);
	int (*XAllowEvents)(Display*, int, Time);
	int (*XAutoRepeatOff)(Display*);
	int (*XAutoRepeatOn)(Display*);
	int (*XBell)(Display*, int);
	int (*XBitmapBitOrder)(Display*);
	int (*XBitmapPad)(Display*);
	int (*XBitmapUnit)(Display*);
	int (*XCellsOfScreen)(Screen*);
	int (*XChangeActivePointerGrab)(Display*, unsigned int, Cursor, Time);
	int (*XChangeGC)(Display*, GC, unsigned long, XGCValues*);
	int (*XChangeKeyboardControl)(Display*, unsigned long, XKeyboardControl*);
	int (*XChangeKeyboardMapping)(Display*, int, int, KeySym*, int);
	int (*XChangePointerControl)(Display*, Bool, Bool, int, int, int);
	int (*XChangeProperty)(Display*, Window, Atom, Atom, int, int, _Xconst unsigned char*, int);
	int (*XChangeSaveSet)(Display*, Window, int);
	int (*XChangeWindowAttributes)(Display*, Window, unsigned long, XSetWindowAttributes*);
	Bool (*XCheckIfEvent)(Display*, XEvent*, Bool (*)(Display*, XEvent*, XPointer), XPointer);
	Bool (*XCheckMaskEvent)(Display*, long, XEvent*);
	Bool (*XCheckTypedEvent)(Display*, int, XEvent*);
	Bool (*XCheckTypedWindowEvent)(Display*, Window, int, XEvent*);
	Bool (*XCheckWindowEvent)(Display*, Window, long, XEvent*);
	int (*XCirculateSubwindows)(Display*, Window, int);
	int (*XCirculateSubwindowsDown)(Display*, Window);
	int (*XCirculateSubwindowsUp)(Display*, Window);
	int (*XClearArea)(Display*, Window, int, int, unsigned int, unsigned int, Bool);
	int (*XClearWindow)(Display*, Window);
	int (*XCloseDisplay)(Display*);
	int (*XConfigureWindow)(Display*, Window, unsigned int, XWindowChanges*);
	int (*XConnectionNumber)(Display*);
	int (*XConvertSelection)(Display*, Atom, Atom, Atom, Window, Time);
	int (*XCopyArea)(Display*, Drawable, Drawable, GC, int, int, unsigned int, unsigned int, int, int);
	int (*XCopyGC)(Display*, GC, unsigned long, GC);
	int (*XCopyPlane)(Display*, Drawable, Drawable, GC, int, int, unsigned int, unsigned int, int, int, unsigned long);
	int (*XDefaultDepth)(Display*, int);
	int (*XDefaultDepthOfScreen)(Screen*);
	int (*XDefaultScreen)(Display*);
	int (*XDefineCursor)(Display*, Window, Cursor);
	int (*XDeleteProperty)(Display*, Window, Atom);
	int (*XDestroyWindow)(Display*, Window);
	int (*XDestroySubwindows)(Display*, Window);
	int (*XDoesBackingStore)(Screen*);
	Bool (*XDoesSaveUnders)(Screen*);
	int (*XDisableAccessControl)(Display*);
	int (*XDisplayCells)(Display*, int);
	int (*XDisplayHeight)(Display*, int);
	int (*XDisplayHeightMM)(Display*, int);
	int (*XDisplayKeycodes)(Display*, int*, int*);
	int (*XDisplayPlanes)(Display*, int);
	int (*XDisplayWidth)(Display*, int);
	int (*XDisplayWidthMM)(Display*, int);
	int (*XDrawArc)(Display*, Drawable, GC, int, int, unsigned int, unsigned int, int, int);
	int (*XDrawArcs)(Display*, Drawable, GC, XArc*, int);
	int (*XDrawImageString)(Display*, Drawable, GC, int, int, _Xconst char*, int);
	int (*XDrawImageString16)(Display*, Drawable, GC, int, int, _Xconst XChar2b*, int);
	int (*XDrawLine)(Display*, Drawable, GC, int, int, int, int);
	int (*XDrawLines)(Display*, Drawable, GC, XPoint*, int, int);
	int (*XDrawPoint)(Display*, Drawable, GC, int, int);
	int (*XDrawPoints)(Display*, Drawable, GC, XPoint*, int, int);
	int (*XDrawRectangle)(Display*, Drawable, GC, int, int, unsigned int, unsigned int);
	int (*XDrawRectangles)(Display*, Drawable, GC, XRectangle*, int);
	int (*XDrawSegments)(Display*, Drawable, GC, XSegment*, int);
	int (*XDrawString)(Display*, Drawable, GC, int, int, _Xconst char*, int);
	int (*XDrawString16)(Display*, Drawable, GC, int, int, _Xconst XChar2b*, int);
	int (*XDrawText)(Display*, Drawable, GC, int, int, XTextItem*, int);
	int (*XDrawText16)(Display*, Drawable, GC, int, int, XTextItem16*, int);
	int (*XEnableAccessControl)(Display*);
	int (*XEventsQueued)(Display*, int);
	Status (*XFetchName)(Display*, Window, char**);
	int (*XFillArc)(Display*, Drawable, GC, int, int, unsigned int, unsigned int, int, int);
	int (*XFillArcs)(Display*, Drawable, GC, XArc*, int);
	int (*XFillPolygon)(Display*, Drawable, GC, XPoint*, int, int, int);
	int (*XFillRectangle)(Display*, Drawable, GC, int, int, unsigned int, unsigned int);
	int (*XFillRectangles)(Display*, Drawable, GC, XRectangle*, int);
	int (*XFlush)(Display*);
	int (*XForceScreenSaver)(Display*, int);
	int (*XFree)(void*);
	int (*XFreeColormap)(Display*, Colormap);
	int (*XFreeColors)(Display*, Colormap, unsigned long*, int, unsigned long);
	int (*XFreeCursor)(Display*, Cursor);
	int (*XFreeExtensionList)(char**);
	int (*XFreeFont)(Display*, XFontStruct*);
	int (*XFreeFontInfo)(char**, XFontStruct*, int);
	int (*XFreeFontNames)(char**);
	int (*XFreeFontPath)(char**);
	int (*XFreeGC)(Display*, GC);
	int (*XFreeModifiermap)(XModifierKeymap*);
	int (*XFreePixmap)(Display*, Pixmap);
	int (*XGeometry)(Display*, int, _Xconst char*, _Xconst char*, unsigned int, unsigned int, unsigned int, int, int, int*, int*, int*, int*);
	int (*XGetErrorDatabaseText)(Display*, _Xconst char*, _Xconst char*, _Xconst char*, char*, int);
	int (*XGetErrorText)(Display*, int, char*, int);
	Bool (*XGetFontProperty)(XFontStruct*, Atom, unsigned long*);
	Status (*XGetGCValues)(Display*, GC, unsigned long, XGCValues*);
	Status (*XGetGeometry)(Display*, Drawable, Window*, int*, int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*);
	Status (*XGetIconName)(Display*, Window, char**);
	int (*XGetInputFocus)(Display*, Window*, int*);
	int (*XGetKeyboardControl)(Display*, XKeyboardState*);
	int (*XGetPointerControl)(Display*, int*, int*, int*);
	int (*XGetPointerMapping)(Display*, unsigned char*, int);
	int (*XGetScreenSaver)(Display*, int*, int*, int*, int*);
	Status (*XGetTransientForHint)(Display*, Window, Window*);
	int (*XGetWindowProperty)(Display*, Window, Atom, long, long, Bool, Atom, Atom*, int*, unsigned long*, unsigned long*, unsigned char**);
	Status (*XGetWindowAttributes)(Display*, Window, XWindowAttributes*);
	int (*XGrabButton)(Display*, unsigned int, unsigned int, Window, Bool, unsigned int, int, int, Window, Cursor);
	int (*XGrabKey)(Display*, int, unsigned int, Window, Bool, int, int);
	int (*XGrabKeyboard)(Display*, Window, Bool, int, int, Time);
	int (*XGrabPointer)(Display*, Window, Bool, unsigned int, int, int, Window, Cursor, Time);
	int (*XGrabServer)(Display*);
	int (*XHeightMMOfScreen)(Screen*);
	int (*XHeightOfScreen)(Screen*);
	int (*XIfEvent)(Display*, XEvent*, Bool (*)(Display*, XEvent*, XPointer), XPointer);
	int (*XImageByteOrder)(Display*);
	int (*XInstallColormap)(Display*, Colormap);
	KeyCode (*XKeysymToKeycode)(Display*, KeySym);
	int (*XKillClient)(Display*, XID);
	Status (*XLookupColor)(Display*, Colormap, _Xconst char*, XColor*, XColor*);
	int (*XLowerWindow)(Display*, Window);
	int (*XMapRaised)(Display*, Window);
	int (*XMapSubwindows)(Display*, Window);
	int (*XMapWindow)(Display*, Window);
	int (*XMaskEvent)(Display*, long, XEvent*);
	int (*XMaxCmapsOfScreen)(Screen*);
	int (*XMinCmapsOfScreen)(Screen*);
	int (*XMoveResizeWindow)(Display*, Window, int, int, unsigned int, unsigned int);
	int (*XMoveWindow)(Display*, Window, int, int);
	int (*XNextEvent)(Display*, XEvent*);
	int (*XNoOp)(Display*);
	Status (*XParseColor)(Display*, Colormap, _Xconst char*, XColor*);
	int (*XParseGeometry)(_Xconst char*, int*, int*, unsigned int*, unsigned int*);
	int (*XPeekEvent)(Display*, XEvent*);
	int (*XPeekIfEvent)(Display*, XEvent*, Bool (*)(Display*, XEvent*, XPointer), XPointer);
	int (*XPending)(Display*);
	int (*XPlanesOfScreen)(Screen*);
	int (*XProtocolRevision)(Display*);
	int (*XProtocolVersion)(Display*);
	int (*XPutBackEvent)(Display*, XEvent*);
	int (*XPutImage)(Display*, Drawable, GC, XImage*, int, int, int, int, unsigned int, unsigned int);
	int (*XQLength)(Display*);
	Status (*XQueryBestCursor)(Display*, Drawable, unsigned int, unsigned int, unsigned int*, unsigned int*);
	Status (*XQueryBestSize)(Display*, int, Drawable, unsigned int, unsigned int, unsigned int*, unsigned int*);
	Status (*XQueryBestStipple)(Display*, Drawable, unsigned int, unsigned int, unsigned int*, unsigned int*);
	Status (*XQueryBestTile)(Display*, Drawable, unsigned int, unsigned int, unsigned int*, unsigned int*);
	int (*XQueryColor)(Display*, Colormap, XColor*);
	int (*XQueryColors)(Display*, Colormap, XColor*, int);
	Bool (*XQueryExtension)(Display*, _Xconst char*, int*, int*, int*);
	int (*XQueryKeymap)(Display*, char[32]);
	Bool (*XQueryPointer)(Display*, Window, Window*, Window*, int*, int*, int*, int*, unsigned int*);
	int (*XQueryTextExtents)(Display*, XID, _Xconst char*, int, int*, int*, int*, XCharStruct*);
	int (*XQueryTextExtents16)(Display*, XID, _Xconst XChar2b*, int, int*, int*, int*, XCharStruct*);
	Status (*XQueryTree)(Display*, Window, Window*, Window*, Window**, unsigned int*);
	int (*XRaiseWindow)(Display*, Window);
	int (*XReadBitmapFile)(Display*, Drawable, _Xconst char*, unsigned int*, unsigned int*, Pixmap*, int*, int*);
	int (*XReadBitmapFileData)(_Xconst char*, unsigned int*, unsigned int*, unsigned char**, int*, int*);
	int (*XRebindKeysym)(Display*, KeySym, KeySym*, int, _Xconst unsigned char*, int);
	int (*XRecolorCursor)(Display*, Cursor, XColor*, XColor*);
	int (*XRefreshKeyboardMapping)(XMappingEvent*);
	int (*XRemoveFromSaveSet)(Display*, Window);
	int (*XRemoveHost)(Display*, XHostAddress*);
	int (*XRemoveHosts)(Display*, XHostAddress*, int);
	int (*XReparentWindow)(Display*, Window, Window, int, int);
	int (*XResetScreenSaver)(Display*);
	int (*XResizeWindow)(Display*, Window, unsigned int, unsigned int);
	int (*XRestackWindows)(Display*, Window*, int);
	int (*XRotateBuffers)(Display*, int);
	int (*XRotateWindowProperties)(Display*, Window, Atom*, int, int);
	int (*XScreenCount)(Display*);
	int (*XSelectInput)(Display*, Window, long);
	Status (*XSendEvent)(Display*, Window, Bool, long, XEvent*);
	int (*XSetAccessControl)(Display*, int);
	int (*XSetArcMode)(Display*, GC, int);
	int (*XSetBackground)(Display*, GC, unsigned long);
	int (*XSetClipMask)(Display*, GC, Pixmap);
	int (*XSetClipOrigin)(Display*, GC, int, int);
	int (*XSetClipRectangles)(Display*, GC, int, int, XRectangle*, int, int);
	int (*XSetCloseDownMode)(Display*, int);
	int (*XSetCommand)(Display*, Window, char**, int);
	int (*XSetDashes)(Display*, GC, int, _Xconst char*, int);
	int (*XSetFillRule)(Display*, GC, int);
	int (*XSetFillStyle)(Display*, GC, int);
	int (*XSetFont)(Display*, GC, Font);
	int (*XSetFontPath)(Display*, char**, int);
	int (*XSetForeground)(Display*, GC, unsigned long);
	int (*XSetFunction)(Display*, GC, int);
	int (*XSetGraphicsExposures)(Display*, GC, Bool);
	int (*XSetIconName)(Display*, Window, _Xconst char*);
	int (*XSetInputFocus)(Display*, Window, int, Time);
	int (*XSetLineAttributes)(Display*, GC, unsigned int, int, int, int);
	int (*XSetModifierMapping)(Display*, XModifierKeymap*);
	int (*XSetPlaneMask)(Display*, GC, unsigned long);
	int (*XSetPointerMapping)(Display*, _Xconst unsigned char*, int);
	int (*XSetScreenSaver)(Display*, int, int, int, int);
	int (*XSetSelectionOwner)(Display*, Atom, Window, Time);
	int (*XSetState)(Display*, GC, unsigned long, unsigned long, int, unsigned long);
	int (*XSetStipple)(Display*, GC, Pixmap);
	int (*XSetSubwindowMode)(Display*, GC, int);
	int (*XSetTSOrigin)(Display*, GC, int, int);
	int (*XSetTile)(Display*, GC, Pixmap);
	int (*XSetWindowBackground)(Display*, Window, unsigned long);
	int (*XSetWindowBackgroundPixmap)(Display*, Window, Pixmap);
	int (*XSetWindowBorder)(Display*, Window, unsigned long);
	int (*XSetWindowBorderPixmap)(Display*, Window, Pixmap);
	int (*XSetWindowBorderWidth)(Display*, Window, unsigned int);
	int (*XSetWindowColormap)(Display*, Window, Colormap);
	int (*XStoreBuffer)(Display*, _Xconst char*, int, int);
	int (*XStoreBytes)(Display*, _Xconst char*, int);
	int (*XStoreColor)(Display*, Colormap, XColor*);
	int (*XStoreColors)(Display*, Colormap, XColor*, int);
	int (*XStoreName)(Display*, Window, _Xconst char*);
	int (*XStoreNamedColor)(Display*, Colormap, _Xconst char*, unsigned long, int);
	int (*XSync)(Display*, Bool);
	int (*XTextExtents)(XFontStruct*, _Xconst char*, int, int*, int*, int*, XCharStruct*);
	int (*XTextExtents16)(XFontStruct*, _Xconst XChar2b*, int, int*, int*, int*, XCharStruct*);
	int (*XTextWidth)(XFontStruct*, _Xconst char*, int);
	int (*XTextWidth16)(XFontStruct*, _Xconst XChar2b*, int);
	Bool (*XTranslateCoordinates)(Display*, Window, Window, int, int, int*, int*, Window*);
	int (*XUndefineCursor)(Display*, Window);
	int (*XUngrabButton)(Display*, unsigned int, unsigned int, Window);
	int (*XUngrabKey)(Display*, int, unsigned int, Window);
	int (*XUngrabKeyboard)(Display*, Time);
	int (*XUngrabPointer)(Display*, Time);
	int (*XUngrabServer)(Display*);
	int (*XUninstallColormap)(Display*, Colormap);
	int (*XUnloadFont)(Display*, Font);
	int (*XUnmapSubwindows)(Display*, Window);
	int (*XUnmapWindow)(Display*, Window);
	int (*XVendorRelease)(Display*);
	int (*XWarpPointer)(Display*, Window, Window, int, int, unsigned int, unsigned int, int, int);
	int (*XWidthMMOfScreen)(Screen*);
	int (*XWidthOfScreen)(Screen*);
	int (*XWindowEvent)(Display*, Window, long, XEvent*);
	int (*XWriteBitmapFile)(Display*, _Xconst char*, Pixmap, unsigned int, unsigned int, int, int);
	Bool (*XSupportsLocale)(void);
	char* (*XSetLocaleModifiers)(const char*);
	XOM (*XOpenOM)(Display*, struct _XrmHashBucketRec*, _Xconst char*, _Xconst char*);
	Status (*XCloseOM)(XOM);
	char* (*XSetOMValues)(XOM, ...)_X_SENTINEL(0);
	char* (*XGetOMValues)(XOM, ...)_X_SENTINEL(0);
	Display* (*XDisplayOfOM)(XOM);
	char* (*XLocaleOfOM)(XOM);
	XOC (*XCreateOC)(XOM, ...) _X_SENTINEL(0);
	void (*XDestroyOC)(XOC);
	XOM (*XOMOfOC)(XOC);
	char* (*XSetOCValues)(XOC, ...)_X_SENTINEL(0);
	char* (*XGetOCValues)(XOC, ...)_X_SENTINEL(0);
	XFontSet (*XCreateFontSet)(Display*, _Xconst char*, char***, int*, char**);
	void (*XFreeFontSet)(Display*, XFontSet);
	int (*XFontsOfFontSet)(XFontSet, XFontStruct***, char***);
	char* (*XBaseFontNameListOfFontSet)(XFontSet);
	char* (*XLocaleOfFontSet)(XFontSet);
	Bool (*XContextDependentDrawing)(XFontSet);
	Bool (*XDirectionalDependentDrawing)(XFontSet);
	Bool (*XContextualDrawing)(XFontSet);
	XFontSetExtents* (*XExtentsOfFontSet)(XFontSet);
	int (*XmbTextEscapement)(XFontSet, _Xconst char*, int);
	int (*XwcTextEscapement)(XFontSet, _Xconst wchar_t*, int);
	int (*Xutf8TextEscapement)(XFontSet, _Xconst char*, int);
	int (*XmbTextExtents)(XFontSet, _Xconst char*, int, XRectangle*, XRectangle*);
	int (*XwcTextExtents)(XFontSet, _Xconst wchar_t*, int, XRectangle*, XRectangle*);
	int (*Xutf8TextExtents)(XFontSet, _Xconst char*, int, XRectangle*, XRectangle*);
	Status (*XmbTextPerCharExtents)(XFontSet, _Xconst char*, int, XRectangle*, XRectangle*, int, int*, XRectangle*, XRectangle*);
	Status (*XwcTextPerCharExtents)(XFontSet, _Xconst wchar_t*, int, XRectangle*, XRectangle*, int, int*, XRectangle*, XRectangle*);
	Status (*Xutf8TextPerCharExtents)(XFontSet, _Xconst char*, int, XRectangle*, XRectangle*, int, int*, XRectangle*, XRectangle*);
	void (*XmbDrawText)(Display*, Drawable, GC, int, int, XmbTextItem*, int);
	void (*XwcDrawText)(Display*, Drawable, GC, int, int, XwcTextItem*, int);
	void (*Xutf8DrawText)(Display*, Drawable, GC, int, int, XmbTextItem*, int);
	void (*XmbDrawString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst char*, int);
	void (*XwcDrawString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst wchar_t*, int);
	void (*Xutf8DrawString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst char*, int);
	void (*XmbDrawImageString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst char*, int);
	void (*XwcDrawImageString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst wchar_t*, int);
	void (*Xutf8DrawImageString)(Display*, Drawable, XFontSet, GC, int, int, _Xconst char*, int);
	XIM (*XOpenIM)(Display*, struct _XrmHashBucketRec*, char*, char*);
	Status (*XCloseIM)(XIM);
	char* (*XGetIMValues)(XIM, ...)_X_SENTINEL(0);
	char* (*XSetIMValues)(XIM, ...)_X_SENTINEL(0);
	Display* (*XDisplayOfIM)(XIM);
	char* (*XLocaleOfIM)(XIM /* im*/);
	XIC (*XCreateIC)(XIM, ...) _X_SENTINEL(0);
	void (*XDestroyIC)(XIC);
	void (*XSetICFocus)(XIC);
	void (*XUnsetICFocus)(XIC);
	wchar_t* (*XwcResetIC)(XIC);
	char* (*XmbResetIC)(XIC);
	char* (*Xutf8ResetIC)(XIC);
	char* (*XSetICValues)(XIC, ...)_X_SENTINEL(0);
	char* (*XGetICValues)(XIC, ...)_X_SENTINEL(0);
	XIM (*XIMOfIC)(XIC);
	Bool (*XFilterEvent)(XEvent*, Window);
	int (*XmbLookupString)(XIC, XKeyPressedEvent*, char*, int, KeySym*, Status*);
	int (*XwcLookupString)(XIC, XKeyPressedEvent*, wchar_t*, int, KeySym*, Status*);
	int (*Xutf8LookupString)(XIC, XKeyPressedEvent*, char*, int, KeySym*, Status*);
	XVaNestedList (*XVaCreateNestedList)(int, ...) _X_SENTINEL(0);
	Bool (*XRegisterIMInstantiateCallback)(Display*, struct _XrmHashBucketRec*, char*, char*, XIDProc, XPointer);
	Bool (*XUnregisterIMInstantiateCallback)(Display*, struct _XrmHashBucketRec*, char*, char*, XIDProc, XPointer);
	Status (*XInternalConnectionNumbers)(Display*, int**, int*);
	void (*XProcessInternalConnection)(Display*, int);
	Status (*XAddConnectionWatch)(Display*, XConnectionWatchProc, XPointer);
	void (*XRemoveConnectionWatch)(Display*, XConnectionWatchProc, XPointer);
	void (*XSetAuthorization)(char*, int, char*, int);
	int (*_Xmbtowc)(wchar_t*, char*, int);
	int (*_Xwctomb)(char*, wchar_t);
	Bool (*XGetEventData)(Display*, XGenericEventCookie*);
	void (*XFreeEventData)(Display*, XGenericEventCookie*);

	XClassHint* (*XAllocClassHint)(void);
	XIconSize* (*XAllocIconSize)(void);
	XSizeHints* (*XAllocSizeHints)(void);
	XStandardColormap* (*XAllocStandardColormap)(void);
	XWMHints* (*XAllocWMHints)(void);
	int (*XClipBox)(Region, XRectangle*);
	Region (*XCreateRegion)(void);
	const char* (*XDefaultString)(void);
	int (*XDeleteContext)(Display*, XID, XContext);
	int (*XDestroyRegion)(Region);
	Bool (*XEmptyRegion)(Region);
	Bool (*XEqualRegion)(Region, Region);
	int (*XFindContext)(Display*, XID, XContext, XPointer*);
	Status (*XGetClassHint)(Display*, Window, XClassHint*);
	Status (*XGetIconSizes)(Display*, Window, XIconSize**, int*);
	Status (*XGetNormalHints)(Display*, Window, XSizeHints*);
	Status (*XGetRGBColormaps)(Display*, Window, XStandardColormap**, int*, Atom);
	Status (*XGetSizeHints)(Display*, Window, XSizeHints*, Atom);
	Status (*XGetStandardColormap)(Display*, Window, XStandardColormap*, Atom);
	Status (*XGetTextProperty)(Display*, Window, XTextProperty*, Atom);
	XVisualInfo* (*XGetVisualInfo)(Display*, long, XVisualInfo*, int*);
	Status (*XGetWMClientMachine)(Display*, Window, XTextProperty*);
	XWMHints* (*XGetWMHints)(Display*, Window);
	Status (*XGetWMIconName)(Display*, Window, XTextProperty*);
	Status (*XGetWMName)(Display*, Window, XTextProperty*);
	Status (*XGetWMNormalHints)(Display*, Window, XSizeHints*, long*);
	Status (*XGetWMSizeHints)(Display*, Window, XSizeHints*, long*, Atom);
	Status (*XGetZoomHints)(Display*, Window, XSizeHints*);
	int (*XIntersectRegion)(Region, Region, Region);
	void (*XConvertCase)(KeySym, KeySym*, KeySym*);
	int (*XLookupString)(XKeyEvent*, char*, int, KeySym*, XComposeStatus*);
	Status (*XMatchVisualInfo)(Display*, int, int, int, XVisualInfo*);
	int (*XOffsetRegion)(Region, int, int);
	Bool (*XPointInRegion)(Region, int, int);
	Region (*XPolygonRegion)(XPoint*, int, int);
	int (*XRectInRegion)(Region, int, int, unsigned int, unsigned int);
	int (*XSaveContext)(Display*, XID, XContext, _Xconst char*);
	int (*XSetClassHint)(Display*, Window, XClassHint*);
	int (*XSetIconSizes)(Display*, Window, XIconSize*, int);
	int (*XSetNormalHints)(Display*, Window, XSizeHints*);
	void (*XSetRGBColormaps)(Display*, Window, XStandardColormap*, int, Atom);
	int (*XSetSizeHints)(Display*, Window, XSizeHints*, Atom);
	int (*XSetStandardProperties)(Display*, Window, _Xconst char*, _Xconst char*, Pixmap, char**, int, XSizeHints*);
	void (*XSetTextProperty)(Display*, Window, XTextProperty*, Atom);
	void (*XSetWMClientMachine)(Display*, Window, XTextProperty*);
	int (*XSetWMHints)(Display*, Window, XWMHints*);
	void (*XSetWMIconName)(Display*, Window, XTextProperty*);
	void (*XSetWMName)(Display*, Window, XTextProperty*);
	void (*XSetWMNormalHints)(Display*, Window, XSizeHints*);
	void (*XSetWMProperties)(Display*, Window, XTextProperty*, XTextProperty*, char**, int, XSizeHints*, XWMHints*, XClassHint*);
	void (*XmbSetWMProperties)(Display*, Window, _Xconst char*, _Xconst char*, char**, int, XSizeHints*, XWMHints*, XClassHint*);
	void (*Xutf8SetWMProperties)(Display*, Window, _Xconst char*, _Xconst char*, char**, int, XSizeHints*, XWMHints*, XClassHint*);
	void (*XSetWMSizeHints)(Display*, Window, XSizeHints*, Atom);
	int (*XSetRegion)(Display*, GC, Region);
	void (*XSetStandardColormap)(Display*, Window, XStandardColormap*, Atom);
	int (*XSetZoomHints)(Display*, Window, XSizeHints*);
	int (*XShrinkRegion)(Region, int, int);
	Status (*XStringListToTextProperty)(char**, int, XTextProperty*);
	int (*XSubtractRegion)(Region, Region, Region);
	int (*XmbTextListToTextProperty)(Display* display, char** list, int count, XICCEncodingStyle style, XTextProperty* text_prop_return);
	int (*XwcTextListToTextProperty)(Display* display, wchar_t** list, int count, XICCEncodingStyle style, XTextProperty* text_prop_return);
	int (*Xutf8TextListToTextProperty)(Display* display, char** list, int count, XICCEncodingStyle style, XTextProperty* text_prop_return);
	void (*XwcFreeStringList)(wchar_t** list);
	Status (*XTextPropertyToStringList)(XTextProperty*, char***, int*);
	int (*XmbTextPropertyToTextList)(Display* display, const XTextProperty* text_prop, char*** list_return, int* count_return);
	int (*XwcTextPropertyToTextList)(Display* display, const XTextProperty* text_prop, wchar_t*** list_return, int* count_return);
	int (*Xutf8TextPropertyToTextList)(Display* display, const XTextProperty* text_prop, char*** list_return, int* count_return);
	int (*XUnionRectWithRegion)(XRectangle*, Region, Region);
	int (*XUnionRegion)(Region, Region, Region);
	int (*XWMGeometry)(Display*, int, _Xconst char*, _Xconst char*, unsigned int, XSizeHints*, int*, int*, int*, int*, int*);
	int (*XXorRegion)(Region, Region, Region);

#if USE_XRENDER
	Bool (*XRenderQueryExtension)(Display* dpy, int* event_basep, int* error_basep);

	Status (*XRenderQueryVersion)(Display* dpy, int* major_versionp,
				      int* minor_versionp);

	Status (*XRenderQueryFormats)(Display* dpy);

	int (*XRenderQuerySubpixelOrder)(Display* dpy, int screen);

	Bool (*XRenderSetSubpixelOrder)(Display* dpy, int screen, int subpixel);

	XRenderPictFormat* (*XRenderFindVisualFormat)(Display*	      dpy,
						      _Xconst Visual* visual);

	XRenderPictFormat* (*XRenderFindFormat)(Display* dpy, unsigned long mask,
						_Xconst XRenderPictFormat* templ,
						int			   count);

	XRenderPictFormat* (*XRenderFindStandardFormat)(Display* dpy, int format);

	XIndexValue* (*XRenderQueryPictIndexValues)(Display*		       dpy,
						    _Xconst XRenderPictFormat* format,
						    int*		       num);

	Picture (*XRenderCreatePicture)(Display* dpy, Drawable drawable,
					_Xconst XRenderPictFormat*	  format,
					unsigned long			  valuemask,
					_Xconst XRenderPictureAttributes* attributes);

	void (*XRenderFreePicture)(Display* dpy, Picture picture);

	void (*XRenderChangePicture)(Display* dpy, Picture picture,
				     unsigned long		       valuemask,
				     _Xconst XRenderPictureAttributes* attributes);

	void (*XRenderSetPictureClipRectangles)(Display* dpy, Picture picture, int xOrigin,
						int yOrigin, _Xconst XRectangle* rects,
						int n);

	void (*XRenderSetPictureClipRegion)(Display* dpy, Picture picture, Region r);

	void (*XRenderSetPictureTransform)(Display* dpy, Picture picture,
					   XTransform* transform);

	void (*XRenderComposite)(Display* dpy, int op, Picture src, Picture mask,
				 Picture dst, int src_x, int src_y, int mask_x, int mask_y,
				 int dst_x, int dst_y, unsigned int width,
				 unsigned int height);

	GlyphSet (*XRenderCreateGlyphSet)(Display* dpy, _Xconst XRenderPictFormat* format);

	GlyphSet (*XRenderReferenceGlyphSet)(Display* dpy, GlyphSet existing);

	void (*XRenderFreeGlyphSet)(Display* dpy, GlyphSet glyphset);

	void (*XRenderAddGlyphs)(Display* dpy, GlyphSet glyphset, _Xconst Glyph* gids,
				 _Xconst XGlyphInfo* glyphs, int nglyphs,
				 _Xconst char* images, int nbyte_images);

	void (*XRenderFreeGlyphs)(Display* dpy, GlyphSet glyphset, _Xconst Glyph* gids,
				  int nglyphs);

	void (*XRenderCompositeString8)(Display* dpy, int op, Picture src, Picture dst,
					_Xconst XRenderPictFormat* maskFormat,
					GlyphSet glyphset, int xSrc, int ySrc, int xDst,
					int yDst, _Xconst char* string, int nchar);

	void (*XRenderCompositeString16)(Display* dpy, int op, Picture src, Picture dst,
					 _Xconst XRenderPictFormat* maskFormat,
					 GlyphSet glyphset, int xSrc, int ySrc, int xDst,
					 int yDst, _Xconst unsigned short* string,
					 int nchar);

	void (*XRenderCompositeString32)(Display* dpy, int op, Picture src, Picture dst,
					 _Xconst XRenderPictFormat* maskFormat,
					 GlyphSet glyphset, int xSrc, int ySrc, int xDst,
					 int yDst, _Xconst unsigned int* string,
					 int nchar);

	void (*XRenderCompositeText8)(Display* dpy, int op, Picture src, Picture dst,
				      _Xconst XRenderPictFormat* maskFormat, int xSrc,
				      int ySrc, int xDst, int yDst,
				      _Xconst XGlyphElt8* elts, int nelt);

	void (*XRenderCompositeText16)(Display* dpy, int op, Picture src, Picture dst,
				       _Xconst XRenderPictFormat* maskFormat, int xSrc,
				       int ySrc, int xDst, int yDst,
				       _Xconst XGlyphElt16* elts, int nelt);

	void (*XRenderCompositeText32)(Display* dpy, int op, Picture src, Picture dst,
				       _Xconst XRenderPictFormat* maskFormat, int xSrc,
				       int ySrc, int xDst, int yDst,
				       _Xconst XGlyphElt32* elts, int nelt);

	void (*XRenderFillRectangle)(Display* dpy, int op, Picture dst,
				     _Xconst XRenderColor* color, int x, int y,
				     unsigned int width, unsigned int height);

	void (*XRenderFillRectangles)(Display* dpy, int op, Picture dst,
				      _Xconst XRenderColor* color,
				      _Xconst XRectangle* rectangles, int n_rects);

	void (*XRenderCompositeTrapezoids)(Display* dpy, int op, Picture src, Picture dst,
					   _Xconst XRenderPictFormat* maskFormat, int xSrc,
					   int ySrc, _Xconst XTrapezoid* traps, int ntrap);

	void (*XRenderCompositeTriangles)(Display* dpy, int op, Picture src, Picture dst,
					  _Xconst XRenderPictFormat* maskFormat, int xSrc,
					  int ySrc, _Xconst XTriangle* triangles,
					  int ntriangle);

	void (*XRenderCompositeTriStrip)(Display* dpy, int op, Picture src, Picture dst,
					 _Xconst XRenderPictFormat* maskFormat, int xSrc,
					 int ySrc, _Xconst XPointFixed* points,
					 int npoint);

	void (*XRenderCompositeTriFan)(Display* dpy, int op, Picture src, Picture dst,
				       _Xconst XRenderPictFormat* maskFormat, int xSrc,
				       int ySrc, _Xconst XPointFixed* points, int npoint);

	void (*XRenderCompositeDoublePoly)(Display* dpy, int op, Picture src, Picture dst,
					   _Xconst XRenderPictFormat* maskFormat, int xSrc,
					   int ySrc, int xDst, int yDst,
					   _Xconst XPointDouble* fpoints, int npoints,
					   int winding);

	Status (*XRenderParseColor)(Display* dpy, char* spec, XRenderColor* def);

	Cursor (*XRenderCreateCursor)(Display* dpy, Picture source, unsigned int x,
				      unsigned int y);

	XFilters* (*XRenderQueryFilters)(Display* dpy, Drawable drawable);

	void (*XRenderSetPictureFilter)(Display* dpy, Picture picture, const char* filter,
					XFixed* params, int nparams);

	Cursor (*XRenderCreateAnimCursor)(Display* dpy, int ncursor, XAnimCursor* cursors);

	void (*XRenderAddTraps)(Display* dpy, Picture picture, int xOff, int yOff,
				_Xconst XTrap* traps, int ntrap);

	Picture (*XRenderCreateSolidFill)(Display* dpy, const XRenderColor* color);

	Picture (*XRenderCreateLinearGradient)(Display*		      dpy,
					       const XLinearGradient* gradient,
					       const XFixed*	      stops,
					       const XRenderColor* colors, int nstops);

	Picture (*XRenderCreateRadialGradient)(Display*		      dpy,
					       const XRadialGradient* gradient,
					       const XFixed*	      stops,
					       const XRenderColor* colors, int nstops);

	Picture (*XRenderCreateConicalGradient)(Display*		dpy,
						const XConicalGradient* gradient,
						const XFixed*		stops,
						const XRenderColor* colors, int nstops);
#endif
} xsymtbl;

#define _Xdebug *xsymtbl._Xdebug
#define XLoadQueryFont xsymtbl.XLoadQueryFont
#define XQueryFont xsymtbl.XQueryFont
#define XGetMotionEvents xsymtbl.XGetMotionEvents
#define XDeleteModifiermapEntry xsymtbl.XDeleteModifiermapEntry
#define XGetModifierMapping xsymtbl.XGetModifierMapping
#define XInsertModifiermapEntry xsymtbl.XInsertModifiermapEntry
#define XNewModifiermap xsymtbl.XNewModifiermap
#define XCreateImage xsymtbl.XCreateImage
#define XInitImage xsymtbl.XInitImage
#define XGetImage xsymtbl.XGetImage
#define XGetSubImage xsymtbl.XGetSubImage
#define XOpenDisplay xsymtbl.XOpenDisplay
#define XrmInitialize xsymtbl.XrmInitialize
#define XFetchBytes xsymtbl.XFetchBytes
#define XFetchBuffer xsymtbl.XFetchBuffer
#define XGetAtomName xsymtbl.XGetAtomName
#define XGetAtomNames xsymtbl.XGetAtomNames
#define XGetDefault xsymtbl.XGetDefault
#define XDisplayName xsymtbl.XDisplayName
#define XKeysymToString xsymtbl.XKeysymToString
#define XInternAtom xsymtbl.XInternAtom
#define XInternAtoms xsymtbl.XInternAtoms
#define XCopyColormapAndFree xsymtbl.XCopyColormapAndFree
#define XCreateColormap xsymtbl.XCreateColormap
#define XCreatePixmapCursor xsymtbl.XCreatePixmapCursor
#define XCreateGlyphCursor xsymtbl.XCreateGlyphCursor
#define XCreateFontCursor xsymtbl.XCreateFontCursor
#define XLoadFont xsymtbl.XLoadFont
#define XCreateGC xsymtbl.XCreateGC
#define XGContextFromGC xsymtbl.XGContextFromGC
#define XFlushGC xsymtbl.XFlushGC
#define XCreatePixmap xsymtbl.XCreatePixmap
#define XCreateBitmapFromData xsymtbl.XCreateBitmapFromData
#define XCreatePixmapFromBitmapData xsymtbl.XCreatePixmapFromBitmapData
#define XCreateSimpleWindow xsymtbl.XCreateSimpleWindow
#define XGetSelectionOwner xsymtbl.XGetSelectionOwner
#define XCreateWindow xsymtbl.XCreateWindow
#define XListInstalledColormaps xsymtbl.XListInstalledColormaps
#define XListFonts xsymtbl.XListFonts
#define XListFontsWithInfo xsymtbl.XListFontsWithInfo
#define XGetFontPath xsymtbl.XGetFontPath
#define XListExtensions xsymtbl.XListExtensions
#define XListProperties xsymtbl.XListProperties
#define XListHosts xsymtbl.XListHosts
#define XKeycodeToKeysym xsymtbl.XKeycodeToKeysym
#define XLookupKeysym xsymtbl.XLookupKeysym
#define XGetKeyboardMapping xsymtbl.XGetKeyboardMapping
#define XStringToKeysym xsymtbl.XStringToKeysym
#define XMaxRequestSize xsymtbl.XMaxRequestSize
#define XExtendedMaxRequestSize xsymtbl.XExtendedMaxRequestSize
#define XResourceManagerString xsymtbl.XResourceManagerString
#define XScreenResourceString xsymtbl.XScreenResourceString
#define XDisplayMotionBufferSize xsymtbl.XDisplayMotionBufferSize
#define XVisualIDFromVisual xsymtbl.XVisualIDFromVisual
#define XInitThreads xsymtbl.XInitThreads
#define XFreeThreads xsymtbl.XFreeThreads
#define XLockDisplay xsymtbl.XLockDisplay
#define XUnlockDisplay xsymtbl.XUnlockDisplay
#define XInitExtension xsymtbl.XInitExtension
#define XAddExtension xsymtbl.XAddExtension
#define XFindOnExtensionList xsymtbl.XFindOnExtensionList
#define XEHeadOfExtensionList xsymtbl.XEHeadOfExtensionList
#define XRootWindow xsymtbl.XRootWindow
#define XDefaultRootWindow xsymtbl.XDefaultRootWindow
#define XRootWindowOfScreen xsymtbl.XRootWindowOfScreen
#define XDefaultVisual xsymtbl.XDefaultVisual
#define XDefaultVisualOfScreen xsymtbl.XDefaultVisualOfScreen
#define XDefaultGC xsymtbl.XDefaultGC
#define XDefaultGCOfScreen xsymtbl.XDefaultGCOfScreen
#define XBlackPixel xsymtbl.XBlackPixel
#define XWhitePixel xsymtbl.XWhitePixel
#define XAllPlanes xsymtbl.XAllPlanes
#define XBlackPixelOfScreen xsymtbl.XBlackPixelOfScreen
#define XWhitePixelOfScreen xsymtbl.XWhitePixelOfScreen
#define XNextRequest xsymtbl.XNextRequest
#define XLastKnownRequestProcessed xsymtbl.XLastKnownRequestProcessed
#define XServerVendor xsymtbl.XServerVendor
#define XDisplayString xsymtbl.XDisplayString
#define XDefaultColormap xsymtbl.XDefaultColormap
#define XDefaultColormapOfScreen xsymtbl.XDefaultColormapOfScreen
#define XDisplayOfScreen xsymtbl.XDisplayOfScreen
#define XScreenOfDisplay xsymtbl.XScreenOfDisplay
#define XDefaultScreenOfDisplay xsymtbl.XDefaultScreenOfDisplay
#define XEventMaskOfScreen xsymtbl.XEventMaskOfScreen
#define XScreenNumberOfScreen xsymtbl.XScreenNumberOfScreen
#define XSetErrorHandler xsymtbl.XSetErrorHandler
#define XSetIOErrorHandler xsymtbl.XSetIOErrorHandler
#define XSetIOErrorExitHandler xsymtbl.XSetIOErrorExitHandler
#define XListPixmapFormats xsymtbl.XListPixmapFormats
#define XListDepths xsymtbl.XListDepths
#define XReconfigureWMWindow xsymtbl.XReconfigureWMWindow
#define XGetWMProtocols xsymtbl.XGetWMProtocols
#define XSetWMProtocols xsymtbl.XSetWMProtocols
#define XIconifyWindow xsymtbl.XIconifyWindow
#define XWithdrawWindow xsymtbl.XWithdrawWindow
#define XGetCommand xsymtbl.XGetCommand
#define XGetWMColormapWindows xsymtbl.XGetWMColormapWindows
#define XSetWMColormapWindows xsymtbl.XSetWMColormapWindows
#define XFreeStringList xsymtbl.XFreeStringList
#define XSetTransientForHint xsymtbl.XSetTransientForHint
#define XActivateScreenSaver xsymtbl.XActivateScreenSaver
#define XAddHost xsymtbl.XAddHost
#define XAddHosts xsymtbl.XAddHosts
#define XAddToExtensionList xsymtbl.XAddToExtensionList
#define XAddToSaveSet xsymtbl.XAddToSaveSet
#define XAllocColor xsymtbl.XAllocColor
#define XAllocColorCells xsymtbl.XAllocColorCells
#define XAllocColorPlanes xsymtbl.XAllocColorPlanes
#define XAllocNamedColor xsymtbl.XAllocNamedColor
#define XAllowEvents xsymtbl.XAllowEvents
#define XAutoRepeatOff xsymtbl.XAutoRepeatOff
#define XAutoRepeatOn xsymtbl.XAutoRepeatOn
#define XBell xsymtbl.XBell
#define XBitmapBitOrder xsymtbl.XBitmapBitOrder
#define XBitmapPad xsymtbl.XBitmapPad
#define XBitmapUnit xsymtbl.XBitmapUnit
#define XCellsOfScreen xsymtbl.XCellsOfScreen
#define XChangeActivePointerGrab xsymtbl.XChangeActivePointerGrab
#define XChangeGC xsymtbl.XChangeGC
#define XChangeKeyboardControl xsymtbl.XChangeKeyboardControl
#define XChangeKeyboardMapping xsymtbl.XChangeKeyboardMapping
#define XChangePointerControl xsymtbl.XChangePointerControl
#define XChangeProperty xsymtbl.XChangeProperty
#define XChangeSaveSet xsymtbl.XChangeSaveSet
#define XChangeWindowAttributes xsymtbl.XChangeWindowAttributes
#define XCheckIfEvent xsymtbl.XCheckIfEvent
#define XCheckMaskEvent xsymtbl.XCheckMaskEvent
#define XCheckTypedEvent xsymtbl.XCheckTypedEvent
#define XCheckTypedWindowEvent xsymtbl.XCheckTypedWindowEvent
#define XCheckWindowEvent xsymtbl.XCheckWindowEvent
#define XCirculateSubwindows xsymtbl.XCirculateSubwindows
#define XCirculateSubwindowsDown xsymtbl.XCirculateSubwindowsDown
#define XCirculateSubwindowsUp xsymtbl.XCirculateSubwindowsUp
#define XClearArea xsymtbl.XClearArea
#define XClearWindow xsymtbl.XClearWindow
#define XCloseDisplay xsymtbl.XCloseDisplay
#define XConfigureWindow xsymtbl.XConfigureWindow
#define XConnectionNumber xsymtbl.XConnectionNumber
#define XConvertSelection xsymtbl.XConvertSelection
#define XCopyArea xsymtbl.XCopyArea
#define XCopyGC xsymtbl.XCopyGC
#define XCopyPlane xsymtbl.XCopyPlane
#define XDefaultDepth xsymtbl.XDefaultDepth
#define XDefaultDepthOfScreen xsymtbl.XDefaultDepthOfScreen
#define XDefaultScreen xsymtbl.XDefaultScreen
#define XDefineCursor xsymtbl.XDefineCursor
#define XDeleteProperty xsymtbl.XDeleteProperty
#define XDestroyWindow xsymtbl.XDestroyWindow
#define XDestroySubwindows xsymtbl.XDestroySubwindows
#define XDoesBackingStore xsymtbl.XDoesBackingStore
#define XDoesSaveUnders xsymtbl.XDoesSaveUnders
#define XDisableAccessControl xsymtbl.XDisableAccessControl
#define XDisplayCells xsymtbl.XDisplayCells
#define XDisplayHeight xsymtbl.XDisplayHeight
#define XDisplayHeightMM xsymtbl.XDisplayHeightMM
#define XDisplayKeycodes xsymtbl.XDisplayKeycodes
#define XDisplayPlanes xsymtbl.XDisplayPlanes
#define XDisplayWidth xsymtbl.XDisplayWidth
#define XDisplayWidthMM xsymtbl.XDisplayWidthMM
#define XDrawArc xsymtbl.XDrawArc
#define XDrawArcs xsymtbl.XDrawArcs
#define XDrawImageString xsymtbl.XDrawImageString
#define XDrawImageString16 xsymtbl.XDrawImageString16
#define XDrawLine xsymtbl.XDrawLine
#define XDrawLines xsymtbl.XDrawLines
#define XDrawPoint xsymtbl.XDrawPoint
#define XDrawPoints xsymtbl.XDrawPoints
#define XDrawRectangle xsymtbl.XDrawRectangle
#define XDrawRectangles xsymtbl.XDrawRectangles
#define XDrawSegments xsymtbl.XDrawSegments
#define XDrawString xsymtbl.XDrawString
#define XDrawString16 xsymtbl.XDrawString16
#define XDrawText xsymtbl.XDrawText
#define XDrawText16 xsymtbl.XDrawText16
#define XEnableAccessControl xsymtbl.XEnableAccessControl
#define XEventsQueued xsymtbl.XEventsQueued
#define XFetchName xsymtbl.XFetchName
#define XFillArc xsymtbl.XFillArc
#define XFillArcs xsymtbl.XFillArcs
#define XFillPolygon xsymtbl.XFillPolygon
#define XFillRectangle xsymtbl.XFillRectangle
#define XFillRectangles xsymtbl.XFillRectangles
#define XFlush xsymtbl.XFlush
#define XForceScreenSaver xsymtbl.XForceScreenSaver
#define XFree xsymtbl.XFree
#define XFreeColormap xsymtbl.XFreeColormap
#define XFreeColors xsymtbl.XFreeColors
#define XFreeCursor xsymtbl.XFreeCursor
#define XFreeExtensionList xsymtbl.XFreeExtensionList
#define XFreeFont xsymtbl.XFreeFont
#define XFreeFontInfo xsymtbl.XFreeFontInfo
#define XFreeFontNames xsymtbl.XFreeFontNames
#define XFreeFontPath xsymtbl.XFreeFontPath
#define XFreeGC xsymtbl.XFreeGC
#define XFreeModifiermap xsymtbl.XFreeModifiermap
#define XFreePixmap xsymtbl.XFreePixmap
#define XGeometry xsymtbl.XGeometry
#define XGetErrorDatabaseText xsymtbl.XGetErrorDatabaseText
#define XGetErrorText xsymtbl.XGetErrorText
#define XGetFontProperty xsymtbl.XGetFontProperty
#define XGetGCValues xsymtbl.XGetGCValues
#define XGetGeometry xsymtbl.XGetGeometry
#define XGetIconName xsymtbl.XGetIconName
#define XGetInputFocus xsymtbl.XGetInputFocus
#define XGetKeyboardControl xsymtbl.XGetKeyboardControl
#define XGetPointerControl xsymtbl.XGetPointerControl
#define XGetPointerMapping xsymtbl.XGetPointerMapping
#define XGetScreenSaver xsymtbl.XGetScreenSaver
#define XGetTransientForHint xsymtbl.XGetTransientForHint
#define XGetWindowProperty xsymtbl.XGetWindowProperty
#define XGetWindowAttributes xsymtbl.XGetWindowAttributes
#define XGrabButton xsymtbl.XGrabButton
#define XGrabKey xsymtbl.XGrabKey
#define XGrabKeyboard xsymtbl.XGrabKeyboard
#define XGrabPointer xsymtbl.XGrabPointer
#define XGrabServer xsymtbl.XGrabServer
#define XHeightMMOfScreen xsymtbl.XHeightMMOfScreen
#define XHeightOfScreen xsymtbl.XHeightOfScreen
#define XIfEvent xsymtbl.XIfEvent
#define XImageByteOrder xsymtbl.XImageByteOrder
#define XInstallColormap xsymtbl.XInstallColormap
#define XKeysymToKeycode xsymtbl.XKeysymToKeycode
#define XKillClient xsymtbl.XKillClient
#define XLookupColor xsymtbl.XLookupColor
#define XLowerWindow xsymtbl.XLowerWindow
#define XMapRaised xsymtbl.XMapRaised
#define XMapSubwindows xsymtbl.XMapSubwindows
#define XMapWindow xsymtbl.XMapWindow
#define XMaskEvent xsymtbl.XMaskEvent
#define XMaxCmapsOfScreen xsymtbl.XMaxCmapsOfScreen
#define XMinCmapsOfScreen xsymtbl.XMinCmapsOfScreen
#define XMoveResizeWindow xsymtbl.XMoveResizeWindow
#define XMoveWindow xsymtbl.XMoveWindow
#define XNextEvent xsymtbl.XNextEvent
#define XNoOp xsymtbl.XNoOp
#define XParseColor xsymtbl.XParseColor
#define XParseGeometry xsymtbl.XParseGeometry
#define XPeekEvent xsymtbl.XPeekEvent
#define XPeekIfEvent xsymtbl.XPeekIfEvent
#define XPending xsymtbl.XPending
#define XPlanesOfScreen xsymtbl.XPlanesOfScreen
#define XProtocolRevision xsymtbl.XProtocolRevision
#define XProtocolVersion xsymtbl.XProtocolVersion
#define XPutBackEvent xsymtbl.XPutBackEvent
#define XPutImage xsymtbl.XPutImage
#define XQLength xsymtbl.XQLength
#define XQueryBestCursor xsymtbl.XQueryBestCursor
#define XQueryBestSize xsymtbl.XQueryBestSize
#define XQueryBestStipple xsymtbl.XQueryBestStipple
#define XQueryBestTile xsymtbl.XQueryBestTile
#define XQueryColor xsymtbl.XQueryColor
#define XQueryColors xsymtbl.XQueryColors
#define XQueryExtension xsymtbl.XQueryExtension
#define XQueryKeymap xsymtbl.XQueryKeymap
#define XQueryPointer xsymtbl.XQueryPointer
#define XQueryTextExtents xsymtbl.XQueryTextExtents
#define XQueryTextExtents16 xsymtbl.XQueryTextExtents16
#define XQueryTree xsymtbl.XQueryTree
#define XRaiseWindow xsymtbl.XRaiseWindow
#define XReadBitmapFile xsymtbl.XReadBitmapFile
#define XReadBitmapFileData xsymtbl.XReadBitmapFileData
#define XRebindKeysym xsymtbl.XRebindKeysym
#define XRecolorCursor xsymtbl.XRecolorCursor
#define XRefreshKeyboardMapping xsymtbl.XRefreshKeyboardMapping
#define XRemoveFromSaveSet xsymtbl.XRemoveFromSaveSet
#define XRemoveHost xsymtbl.XRemoveHost
#define XRemoveHosts xsymtbl.XRemoveHosts
#define XReparentWindow xsymtbl.XReparentWindow
#define XResetScreenSaver xsymtbl.XResetScreenSaver
#define XResizeWindow xsymtbl.XResizeWindow
#define XRestackWindows xsymtbl.XRestackWindows
#define XRotateBuffers xsymtbl.XRotateBuffers
#define XRotateWindowProperties xsymtbl.XRotateWindowProperties
#define XScreenCount xsymtbl.XScreenCount
#define XSelectInput xsymtbl.XSelectInput
#define XSendEvent xsymtbl.XSendEvent
#define XSetAccessControl xsymtbl.XSetAccessControl
#define XSetArcMode xsymtbl.XSetArcMode
#define XSetBackground xsymtbl.XSetBackground
#define XSetClipMask xsymtbl.XSetClipMask
#define XSetClipOrigin xsymtbl.XSetClipOrigin
#define XSetClipRectangles xsymtbl.XSetClipRectangles
#define XSetCloseDownMode xsymtbl.XSetCloseDownMode
#define XSetCommand xsymtbl.XSetCommand
#define XSetDashes xsymtbl.XSetDashes
#define XSetFillRule xsymtbl.XSetFillRule
#define XSetFillStyle xsymtbl.XSetFillStyle
#define XSetFont xsymtbl.XSetFont
#define XSetFontPath xsymtbl.XSetFontPath
#define XSetForeground xsymtbl.XSetForeground
#define XSetFunction xsymtbl.XSetFunction
#define XSetGraphicsExposures xsymtbl.XSetGraphicsExposures
#define XSetIconName xsymtbl.XSetIconName
#define XSetInputFocus xsymtbl.XSetInputFocus
#define XSetLineAttributes xsymtbl.XSetLineAttributes
#define XSetModifierMapping xsymtbl.XSetModifierMapping
#define XSetPlaneMask xsymtbl.XSetPlaneMask
#define XSetPointerMapping xsymtbl.XSetPointerMapping
#define XSetScreenSaver xsymtbl.XSetScreenSaver
#define XSetSelectionOwner xsymtbl.XSetSelectionOwner
#define XSetState xsymtbl.XSetState
#define XSetStipple xsymtbl.XSetStipple
#define XSetSubwindowMode xsymtbl.XSetSubwindowMode
#define XSetTSOrigin xsymtbl.XSetTSOrigin
#define XSetTile xsymtbl.XSetTile
#define XSetWindowBackground xsymtbl.XSetWindowBackground
#define XSetWindowBackgroundPixmap xsymtbl.XSetWindowBackgroundPixmap
#define XSetWindowBorder xsymtbl.XSetWindowBorder
#define XSetWindowBorderPixmap xsymtbl.XSetWindowBorderPixmap
#define XSetWindowBorderWidth xsymtbl.XSetWindowBorderWidth
#define XSetWindowColormap xsymtbl.XSetWindowColormap
#define XStoreBuffer xsymtbl.XStoreBuffer
#define XStoreBytes xsymtbl.XStoreBytes
#define XStoreColor xsymtbl.XStoreColor
#define XStoreColors xsymtbl.XStoreColors
#define XStoreName xsymtbl.XStoreName
#define XStoreNamedColor xsymtbl.XStoreNamedColor
#define XSync xsymtbl.XSync
#define XTextExtents xsymtbl.XTextExtents
#define XTextExtents16 xsymtbl.XTextExtents16
#define XTextWidth xsymtbl.XTextWidth
#define XTextWidth16 xsymtbl.XTextWidth16
#define XTranslateCoordinates xsymtbl.XTranslateCoordinates
#define XUndefineCursor xsymtbl.XUndefineCursor
#define XUngrabButton xsymtbl.XUngrabButton
#define XUngrabKey xsymtbl.XUngrabKey
#define XUngrabKeyboard xsymtbl.XUngrabKeyboard
#define XUngrabPointer xsymtbl.XUngrabPointer
#define XUngrabServer xsymtbl.XUngrabServer
#define XUninstallColormap xsymtbl.XUninstallColormap
#define XUnloadFont xsymtbl.XUnloadFont
#define XUnmapSubwindows xsymtbl.XUnmapSubwindows
#define XUnmapWindow xsymtbl.XUnmapWindow
#define XVendorRelease xsymtbl.XVendorRelease
#define XWarpPointer xsymtbl.XWarpPointer
#define XWidthMMOfScreen xsymtbl.XWidthMMOfScreen
#define XWidthOfScreen xsymtbl.XWidthOfScreen
#define XWindowEvent xsymtbl.XWindowEvent
#define XWriteBitmapFile xsymtbl.XWriteBitmapFile
#define XSupportsLocale xsymtbl.XSupportsLocale
#define XSetLocaleModifiers xsymtbl.XSetLocaleModifiers
#define XOpenOM xsymtbl.XOpenOM
#define XCloseOM xsymtbl.XCloseOM
#define XSetOMValues xsymtbl.XSetOMValues
#define XGetOMValues xsymtbl.XGetOMValues
#define XDisplayOfOM xsymtbl.XDisplayOfOM
#define XLocaleOfOM xsymtbl.XLocaleOfOM
#define XCreateOC xsymtbl.XCreateOC
#define XDestroyOC xsymtbl.XDestroyOC
#define XOMOfOC xsymtbl.XOMOfOC
#define XSetOCValues xsymtbl.XSetOCValues
#define XGetOCValues xsymtbl.XGetOCValues
#define XCreateFontSet xsymtbl.XCreateFontSet
#define XFreeFontSet xsymtbl.XFreeFontSet
#define XFontsOfFontSet xsymtbl.XFontsOfFontSet
#define XBaseFontNameListOfFontSet xsymtbl.XBaseFontNameListOfFontSet
#define XLocaleOfFontSet xsymtbl.XLocaleOfFontSet
#define XContextDependentDrawing xsymtbl.XContextDependentDrawing
#define XDirectionalDependentDrawing xsymtbl.XDirectionalDependentDrawing
#define XContextualDrawing xsymtbl.XContextualDrawing
#define XExtentsOfFontSet xsymtbl.XExtentsOfFontSet
#define XmbTextEscapement xsymtbl.XmbTextEscapement
#define XwcTextEscapement xsymtbl.XwcTextEscapement
#define Xutf8TextEscapement xsymtbl.Xutf8TextEscapement
#define XmbTextExtents xsymtbl.XmbTextExtents
#define XwcTextExtents xsymtbl.XwcTextExtents
#define Xutf8TextExtents xsymtbl.Xutf8TextExtents
#define XmbTextPerCharExtents xsymtbl.XmbTextPerCharExtents
#define XwcTextPerCharExtents xsymtbl.XwcTextPerCharExtents
#define Xutf8TextPerCharExtents xsymtbl.Xutf8TextPerCharExtents
#define XmbDrawText xsymtbl.XmbDrawText
#define XwcDrawText xsymtbl.XwcDrawText
#define Xutf8DrawText xsymtbl.Xutf8DrawText
#define XmbDrawString xsymtbl.XmbDrawString
#define XwcDrawString xsymtbl.XwcDrawString
#define Xutf8DrawString xsymtbl.Xutf8DrawString
#define XmbDrawImageString xsymtbl.XmbDrawImageString
#define XwcDrawImageString xsymtbl.XwcDrawImageString
#define Xutf8DrawImageString xsymtbl.Xutf8DrawImageString
#define XOpenIM xsymtbl.XOpenIM
#define XCloseIM xsymtbl.XCloseIM
#define XGetIMValues xsymtbl.XGetIMValues
#define XSetIMValues xsymtbl.XSetIMValues
#define XDisplayOfIM xsymtbl.XDisplayOfIM
#define XLocaleOfIM xsymtbl.XLocaleOfIM
#define XCreateIC xsymtbl.XCreateIC
#define XDestroyIC xsymtbl.XDestroyIC
#define XSetICFocus xsymtbl.XSetICFocus
#define XUnsetICFocus xsymtbl.XUnsetICFocus
#define XwcResetIC xsymtbl.XwcResetIC
#define XmbResetIC xsymtbl.XmbResetIC
#define Xutf8ResetIC xsymtbl.Xutf8ResetIC
#define XSetICValues xsymtbl.XSetICValues
#define XGetICValues xsymtbl.XGetICValues
#define XIMOfIC xsymtbl.XIMOfIC
#define XFilterEvent xsymtbl.XFilterEvent
#define XmbLookupString xsymtbl.XmbLookupString
#define XwcLookupString xsymtbl.XwcLookupString
#define Xutf8LookupString xsymtbl.Xutf8LookupString
#define XVaCreateNestedList xsymtbl.XVaCreateNestedList
#define XRegisterIMInstantiateCallback xsymtbl.XRegisterIMInstantiateCallback
#define XUnregisterIMInstantiateCallback xsymtbl.XUnregisterIMInstantiateCallback
#define XInternalConnectionNumbers xsymtbl.XInternalConnectionNumbers
#define XProcessInternalConnection xsymtbl.XProcessInternalConnection
#define XAddConnectionWatch xsymtbl.XAddConnectionWatch
#define XRemoveConnectionWatch xsymtbl.XRemoveConnectionWatch
#define XSetAuthorization xsymtbl.XSetAuthorization
#define _Xmbtowc xsymtbl._Xmbtowc
#define _Xwctomb xsymtbl._Xwctomb
#define XGetEventData xsymtbl.XGetEventData
#define XFreeEventData xsymtbl.XFreeEventData

#define XAllocClassHint xsymtbl.XAllocClassHint
#define XAllocIconSize xsymtbl.XAllocIconSize
#define XAllocSizeHints xsymtbl.XAllocSizeHints
#define XAllocStandardColormap xsymtbl.XAllocStandardColormap
#define XAllocWMHints xsymtbl.XAllocWMHints
#define XClipBox xsymtbl.XClipBox
#define XCreateRegion xsymtbl.XCreateRegion
#define XDefaultString xsymtbl.XDefaultString
#define XDeleteContext xsymtbl.XDeleteContext
#define XDestroyRegion xsymtbl.XDestroyRegion
#define XEmptyRegion xsymtbl.XEmptyRegion
#define XEqualRegion xsymtbl.XEqualRegion
#define XFindContext xsymtbl.XFindContext
#define XGetClassHint xsymtbl.XGetClassHint
#define XGetIconSizes xsymtbl.XGetIconSizes
#define XGetNormalHints xsymtbl.XGetNormalHints
#define XGetRGBColormaps xsymtbl.XGetRGBColormaps
#define XGetSizeHints xsymtbl.XGetSizeHints
#define XGetStandardColormap xsymtbl.XGetStandardColormap
#define XGetTextProperty xsymtbl.XGetTextProperty
#define XGetVisualInfo xsymtbl.XGetVisualInfo
#define XGetWMClientMachine xsymtbl.XGetWMClientMachine
#define XGetWMHints xsymtbl.XGetWMHints
#define XGetWMIconName xsymtbl.XGetWMIconName
#define XGetWMName xsymtbl.XGetWMName
#define XGetWMNormalHints xsymtbl.XGetWMNormalHints
#define XGetWMSizeHints xsymtbl.XGetWMSizeHints
#define XGetZoomHints xsymtbl.XGetZoomHints
#define XIntersectRegion xsymtbl.XIntersectRegion
#define XConvertCase xsymtbl.XConvertCase
#define XLookupString xsymtbl.XLookupString
#define XMatchVisualInfo xsymtbl.XMatchVisualInfo
#define XOffsetRegion xsymtbl.XOffsetRegion
#define XPointInRegion xsymtbl.XPointInRegion
#define XPolygonRegion xsymtbl.XPolygonRegion
#define XRectInRegion xsymtbl.XRectInRegion
#define XSaveContext xsymtbl.XSaveContext
#define XSetClassHint xsymtbl.XSetClassHint
#define XSetIconSizes xsymtbl.XSetIconSizes
#define XSetNormalHints xsymtbl.XSetNormalHints
#define XSetRGBColormaps xsymtbl.XSetRGBColormaps
#define XSetSizeHints xsymtbl.XSetSizeHints
#define XSetStandardProperties xsymtbl.XSetStandardProperties
#define XSetTextProperty xsymtbl.XSetTextProperty
#define XSetWMClientMachine xsymtbl.XSetWMClientMachine
#define XSetWMHints xsymtbl.XSetWMHints
#define XSetWMIconName xsymtbl.XSetWMIconName
#define XSetWMName xsymtbl.XSetWMName
#define XSetWMNormalHints xsymtbl.XSetWMNormalHints
#define XSetWMProperties xsymtbl.XSetWMProperties
#define XmbSetWMProperties xsymtbl.XmbSetWMProperties
#define Xutf8SetWMProperties xsymtbl.Xutf8SetWMProperties
#define XSetWMSizeHints xsymtbl.XSetWMSizeHints
#define XSetRegion xsymtbl.XSetRegion
#define XSetStandardColormap xsymtbl.XSetStandardColormap
#define XSetZoomHints xsymtbl.XSetZoomHints
#define XShrinkRegion xsymtbl.XShrinkRegion
#define XStringListToTextProperty xsymtbl.XStringListToTextProperty
#define XSubtractRegion xsymtbl.XSubtractRegion
#define XmbTextListToTextProperty xsymtbl.XmbTextListToTextProperty
#define XwcTextListToTextProperty xsymtbl.XwcTextListToTextProperty
#define Xutf8TextListToTextProperty xsymtbl.Xutf8TextListToTextProperty
#define XwcFreeStringList xsymtbl.XwcFreeStringList
#define XTextPropertyToStringList xsymtbl.XTextPropertyToStringList
#define XmbTextPropertyToTextList xsymtbl.XmbTextPropertyToTextList
#define XwcTextPropertyToTextList xsymtbl.XwcTextPropertyToTextList
#define Xutf8TextPropertyToTextList xsymtbl.Xutf8TextPropertyToTextList
#define XUnionRectWithRegion xsymtbl.XUnionRectWithRegion
#define XUnionRegion xsymtbl.XUnionRegion
#define XWMGeometry xsymtbl.XWMGeometry
#define XXorRegion xsymtbl.XXorRegion

#if USE_XRENDER
#define XRenderQueryExtension xsymtbl.XRenderQueryExtension
#define XRenderQueryVersion xsymtbl.XRenderQueryVersion
#define XRenderQueryFormats xsymtbl.XRenderQueryFormats
#define XRenderQuerySubpixelOrder xsymtbl.XRenderQuerySubpixelOrder
#define XRenderSetSubpixelOrder xsymtbl.XRenderSetSubpixelOrder
#define XRenderFindVisualFormat xsymtbl.XRenderFindVisualFormat
#define XRenderFindFormat xsymtbl.XRenderFindFormat
#define XRenderFindStandardFormat xsymtbl.XRenderFindStandardFormat
#define XRenderQueryPictIndexValues xsymtbl.XRenderQueryPictIndexValues
#define XRenderCreatePicture xsymtbl.XRenderCreatePicture
#define XRenderFreePicture xsymtbl.XRenderFreePicture
#define XRenderChangePicture xsymtbl.XRenderChangePicture
#define XRenderSetPictureClipRectangles xsymtbl.XRenderSetPictureClipRectangles
#define XRenderSetPictureClipRegion xsymtbl.XRenderSetPictureClipRegion
#define XRenderSetPictureTransform xsymtbl.XRenderSetPictureTransform
#define XRenderComposite xsymtbl.XRenderComposite
#define XRenderCreateGlyphSet xsymtbl.XRenderCreateGlyphSet
#define XRenderReferenceGlyphSet xsymtbl.XRenderReferenceGlyphSet
#define XRenderFreeGlyphSet xsymtbl.XRenderFreeGlyphSet
#define XRenderAddGlyphs xsymtbl.XRenderAddGlyphs
#define XRenderFreeGlyphs xsymtbl.XRenderFreeGlyphs
#define XRenderCompositeString8 xsymtbl.XRenderCompositeString8
#define XRenderCompositeString16 xsymtbl.XRenderCompositeString16
#define XRenderCompositeString32 xsymtbl.XRenderCompositeString32
#define XRenderCompositeText8 xsymtbl.XRenderCompositeText8
#define XRenderCompositeText16 xsymtbl.XRenderCompositeText16
#define XRenderCompositeText32 xsymtbl.XRenderCompositeText32
#define XRenderFillRectangle xsymtbl.XRenderFillRectangle
#define XRenderFillRectangles xsymtbl.XRenderFillRectangles
#define XRenderCompositeTrapezoids xsymtbl.XRenderCompositeTrapezoids
#define XRenderCompositeTriangles xsymtbl.XRenderCompositeTriangles
#define XRenderCompositeTriStrip xsymtbl.XRenderCompositeTriStrip
#define XRenderCompositeTriFan xsymtbl.XRenderCompositeTriFan
#define XRenderCompositeDoublePoly xsymtbl.XRenderCompositeDoublePoly
#define XRenderParseColor xsymtbl.XRenderParseColor
#define XRenderCreateCursor xsymtbl.XRenderCreateCursor
#define XRenderQueryFilters xsymtbl.XRenderQueryFilters
#define XRenderSetPictureFilter xsymtbl.XRenderSetPictureFilter
#define XRenderCreateAnimCursor xsymtbl.XRenderCreateAnimCursor
#define XRenderAddTraps xsymtbl.XRenderAddTraps
#define XRenderCreateSolidFill xsymtbl.XRenderCreateSolidFill
#define XRenderCreateLinearGradient xsymtbl.XRenderCreateLinearGradient
#define XRenderCreateRadialGradient xsymtbl.XRenderCreateRadialGradient
#define XRenderCreateConicalGradient xsymtbl.XRenderCreateConicalGradient
#endif

typedef struct mwm_hints {
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long	      input_mode;
	unsigned long status;
} mwm_hints_t;

enum mwm_hints_enum {
	MWM_HINTS_FUNCTIONS   = (1L << 0),
	MWM_HINTS_DECORATIONS = (1L << 1),
	MWM_FUNC_ALL	      = (1L << 0),
	MWM_FUNC_RESIZE	      = (1L << 1),
	MWM_FUNC_MOVE	      = (1L << 2),
	MWM_FUNC_MINIMIZE     = (1L << 3),
	MWM_FUNC_MAXIMIZE     = (1L << 4),
	MWM_FUNC_CLOSE	      = (1L << 5)
};

static unsigned long mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | KeymapNotify | FocusChangeMask;

static void create_pixmap(MwLL handle) {
	XWindowAttributes attr;
	int		  x, y;
	unsigned int	  w, h;

	MwLLGetXYWH(handle, &x, &y, &w, &h);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	handle->x11.pixmap = XCreatePixmap(handle->x11.display, handle->x11.window, w, h, attr.depth);
}

static void destroy_pixmap(MwLL handle) {
	XFreePixmap(handle->x11.display, handle->x11.pixmap);
}

static void sync_move(MwLL handle, int x, int y) {
	XEvent*		  queue = NULL;
	XEvent		  ev;
	unsigned long	  n = MwTimeGetTick() + 100;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);

	XSync(handle->x11.display, False);

	while(!xwa.override_redirect && n > MwTimeGetTick()) {
		XSync(handle->x11.display, False);
		if(!XPending(handle->x11.display)) continue;
		XNextEvent(handle->x11.display, &ev);
		if(ev.type == ReparentNotify && ev.xreparent.window == handle->x11.window) {
			break;
		} else {
			arrput(queue, ev);
		}
	}

	while(arrlen(queue) > 0) {
		XPutBackEvent(handle->x11.display, &queue[0]);
		arrdel(queue, 0);
	}
	arrfree(queue);

	MwLLSetXY(handle, x, y);
}

static void wait_map(MwLL handle, int sync) {
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.map_state != IsViewable) {
		XSync(handle->x11.display, False);

		XMapWindow(handle->x11.display, handle->x11.window);
		XSync(handle->x11.display, False);

		if(!sync) return;
		do {
			XMapWindow(handle->x11.display, handle->x11.window);
			XSync(handle->x11.display, False);
			XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
		} while(xwa.map_state != IsViewable);
	}
}

static void wait_unmap(MwLL handle, int sync) {
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.map_state != IsUnmapped) {
		XSync(handle->x11.display, False);

		XUnmapWindow(handle->x11.display, handle->x11.window);
		XSync(handle->x11.display, False);

		if(!sync) return;
		do {
			XUnmapWindow(handle->x11.display, handle->x11.window);
			XSync(handle->x11.display, False);
			XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
		} while(xwa.map_state != IsUnmapped);
	}
}

static unsigned long generate_color(MwLL handle, unsigned long r, unsigned long g, unsigned long b) {
	unsigned long c = 0;

	c |= (r * handle->x11.red_max / 255) << handle->x11.red_shift;

	c |= (g * handle->x11.green_max / 255) << handle->x11.green_shift;

	c |= (b * handle->x11.blue_max / 255) << handle->x11.blue_shift;

	return c;
}

static XVisualInfo* get_visual_info(Display* display) {
	XVisualInfo xvi;
	int	    n;
	Visual*	    visual = DefaultVisual(display, DefaultScreen(display));

	xvi.visualid = XVisualIDFromVisual(visual);

	return XGetVisualInfo(display, VisualIDMask, &xvi, &n);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL	      r;
	Window	      p;
	XVisualInfo*  xvi;
	unsigned long n = 1;
	int	      i;
	int	      px = x, py = y;
	XSizeHints    sh;

	r = malloc(sizeof(*r));

	MwLLCreateCommon(r);

	if(px == MwDEFAULT) px = 0;
	if(py == MwDEFAULT) py = 0;
	if(width < 2) width = 2;
	if(height < 2) height = 2;

	if(parent == NULL) {
		r->x11.display	= XOpenDisplay(NULL);
		p		= XRootWindow(r->x11.display, XDefaultScreen(r->x11.display));
		r->x11.top	= 1;
		r->x11.toplevel = 1;
	} else {
		r->x11.display	= parent->x11.display;
		p		= parent->x11.window;
		r->x11.top	= 0;
		r->x11.toplevel = 0;
	}
	r->x11.window = XCreateSimpleWindow(r->x11.display, p, px, py, width, height, 0, 0, WhitePixel(r->x11.display, DefaultScreen(r->x11.display)));

	sh.flags       = PWinGravity;
	sh.win_gravity = StaticGravity;
	XSetWMNormalHints(r->x11.display, r->x11.window, &sh);

	xvi = get_visual_info(r->x11.display);

	if(xvi->red_mask != 0) {
		i = 0;
		while(!((n << i) & xvi->red_mask)) i++;
		r->x11.red_mask	 = xvi->red_mask;
		r->x11.red_max	 = xvi->red_mask >> i;
		r->x11.red_shift = i;

		i = 0;
		while(!((n << i) & xvi->green_mask)) i++;
		r->x11.green_mask  = xvi->green_mask;
		r->x11.green_max   = xvi->green_mask >> i;
		r->x11.green_shift = i;

		i = 0;
		while(!((n << i) & xvi->blue_mask)) i++;
		r->x11.blue_mask  = xvi->blue_mask;
		r->x11.blue_max	  = xvi->blue_mask >> i;
		r->x11.blue_shift = i;
	}

	XFree(xvi);

	XSetLocaleModifiers("");
	if((r->x11.xim = XOpenIM(r->x11.display, 0, 0, 0)) == NULL) {
		XSetLocaleModifiers("@im=none");
		r->x11.xim = XOpenIM(r->x11.display, 0, 0, 0);
	}

	r->x11.xic = XCreateIC(r->x11.xim,
			       XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
			       XNClientWindow, r->x11.window,
			       XNFocusWindow, r->x11.window,
			       NULL);
	XSetICFocus(r->x11.xic);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendX11;

	r->x11.width  = width;
	r->x11.height = height;

	r->x11.grabbed	    = 0;
	r->x11.force_render = 0;

	r->x11.colormap	    = DefaultColormap(r->x11.display, XDefaultScreen(r->x11.display));
	r->x11.wm_delete    = XInternAtom(r->x11.display, "WM_DELETE_WINDOW", False);
	r->x11.wm_protocols = XInternAtom(r->x11.display, "WM_PROTOCOLS", False);
	XSetWMProtocols(r->x11.display, r->x11.window, &r->x11.wm_delete, 1);

	r->x11.utf8_string   = XInternAtom(r->x11.display, "UTF8_STRING", False);
	r->x11.compound_text = XInternAtom(r->x11.display, "COMPOUND_TEXT", False);
	r->x11.text	     = XInternAtom(r->x11.display, "TEXT", False);
	r->x11.clipboard     = XInternAtom(r->x11.display, "CLIPBOARD", False);
	r->x11.selection     = XInternAtom(r->x11.display, "_MILSKO_SELECTION_", False);

	r->x11.clipboard_pending = 0;

	r->x11.gc = XCreateGC(r->x11.display, r->x11.window, 0, NULL);

	create_pixmap(r);

	XSetGraphicsExposures(r->x11.display, r->x11.gc, False);

	XSelectInput(r->x11.display, r->x11.window, mask);

	wait_map(r, 0);

	if(x != MwDEFAULT || y != MwDEFAULT) {
		unsigned int dummy;

		MwLLGetXYWH(r, &px, &py, &dummy, &dummy);

		if(x == MwDEFAULT) x = px;
		if(y == MwDEFAULT) y = py;

		if(parent == NULL) {
			sync_move(r, x, y);
		} else {
			MwLLSetXY(r, x, y);
		}
	}

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	if(handle->x11.xic) XDestroyIC(handle->x11.xic);
	if(handle->x11.xim) XCloseIM(handle->x11.xim);

	destroy_pixmap(handle);
	XFreeGC(handle->x11.display, handle->x11.gc);
	XUnmapWindow(handle->x11.display, handle->x11.window);
	XDestroyWindow(handle->x11.display, handle->x11.window);

	XSync(handle->x11.display, False);

	if(handle->x11.toplevel) XCloseDisplay(handle->x11.display);

	free(handle);
}

static void MwLLBeginDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndDrawImpl(MwLL handle) {
	(void)handle;
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int	i;
	XPoint* p = malloc(sizeof(*p) * points_count);

	XSetForeground(handle->x11.display, handle->x11.gc, color->x11.pixel);

	for(i = 0; i < points_count; i++) {
		p[i].x = points[i].x;
		p[i].y = points[i].y;
	}
	XFillPolygon(handle->x11.display, handle->x11.pixmap, handle->x11.gc, p, points_count, Nonconvex, CoordModeOrigin);

	free(p);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	XSetForeground(handle->x11.display, handle->x11.gc, color->x11.pixel);

	XDrawLine(handle->x11.display, handle->x11.pixmap, handle->x11.gc, points[0].x, points[0].y, points[1].x, points[1].y);
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));
	MwLLColorUpdate(handle, c, r, g, b);
	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	XColor xc;

	if(handle->x11.red_mask == 0) {
		if(r > 255) r = 255;
		if(g > 255) g = 255;
		if(b > 255) b = 255;
		if(r < 0) r = 0;
		if(g < 0) g = 0;
		if(b < 0) b = 0;

		xc.red	 = 256 * r;
		xc.green = 256 * g;
		xc.blue	 = 256 * b;
		XAllocColor(handle->x11.display, handle->x11.colormap, &xc);

		c->x11.pixel = xc.pixel;
	} else {
		c->x11.pixel = generate_color(handle, r, g, b);
	}
	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	Window	     root;
	unsigned int border, depth;

	XGetGeometry(handle->x11.display, handle->x11.window, &root, x, y, w, h, &border, &depth);
	if(handle->x11.top) {
		int    rx, ry;
		Window child;

		XTranslateCoordinates(handle->x11.display, handle->x11.window, root, 0, 0, &rx, &ry, &child);

		*x = rx;
		*y = ry;
	}
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	XSizeHints	  sh;
	long		  r;
	XWindowChanges	  xwc;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.x == x && xwa.y == y) return;

	sh.flags = 0;
	XGetWMNormalHints(handle->x11.display, handle->x11.window, &sh, &r);

	sh.flags |= USPosition;
	sh.x = x;
	sh.y = y;

#if 1
	xwc.x = x;
	xwc.y = y;
	XConfigureWindow(handle->x11.display, handle->x11.window, CWX | CWY, &xwc);
#else
	XMoveWindow(handle->x11.display, handle->x11.window, x, y);
#endif
	XSetWMNormalHints(handle->x11.display, handle->x11.window, &sh);

	XSync(handle->x11.display, False);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	XSizeHints	  sh;
	long		  r;
	XWindowChanges	  xwc;
	XWindowAttributes xwa;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);
	if(xwa.width == w && xwa.height == h) return;

	sh.flags = 0;
	XGetWMNormalHints(handle->x11.display, handle->x11.window, &sh, &r);

	if(w < 2) w = 2;
	if(h < 2) h = 2;

	sh.flags |= PSize;
	sh.width  = w;
	sh.height = h;

#if 1
	xwc.width  = w;
	xwc.height = h;
	XConfigureWindow(handle->x11.display, handle->x11.window, CWWidth | CWHeight, &xwc);
#else
	XResizeWindow(handle->x11.display, handle->x11.window, w, h);
#endif
	XSetWMNormalHints(handle->x11.display, handle->x11.window, &sh);

	destroy_pixmap(handle);
	create_pixmap(handle);

	/* we want events */
#if 0
	handle->x11.width  = w;
	handle->x11.height = h;
#endif

	XSync(handle->x11.display, False);

	MwLLForceRender(handle);
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static int MwLLPendingImpl(MwLL handle) {
	XEvent ev;

	if(handle->x11.clipboard_pending && (MwTimeGetTick() - handle->x11.clipboard_time) >= ClipboardTimeout) {
		return 1;
	}

	if(XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, ClientMessage, &ev) || XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, SelectionNotify, &ev) || XCheckWindowEvent(handle->x11.display, handle->x11.window, mask, &ev)) {
		XPutBackEvent(handle->x11.display, &ev);
		return 1;
	}
	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	XEvent ev;

	if(handle->x11.clipboard_pending && (MwTimeGetTick() - handle->x11.clipboard_time) >= ClipboardTimeout) {
		Atom a = handle->x11.selection;

		handle->x11.clipboard_pending++;
		handle->x11.clipboard_time = MwTimeGetTick();

		switch(handle->x11.clipboard_pending) {
		case 2:
			a = handle->x11.compound_text;
			break;
		case 3:
			a = handle->x11.text;
			break;
		case 4:
			a = XA_STRING;
			break;
		case 5:
			handle->x11.clipboard_pending = 0;
			break;
		}

		if(a != handle->x11.selection) {
			XConvertSelection(handle->x11.display, handle->x11.clipboard, a, handle->x11.selection, handle->x11.window, CurrentTime);
		}
	}

	while(XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, ClientMessage, &ev) || XCheckTypedWindowEvent(handle->x11.display, handle->x11.window, SelectionNotify, &ev) || XCheckWindowEvent(handle->x11.display, handle->x11.window, mask, &ev)) {
		int render = 0;
		if(ev.type == Expose) {
			handle->x11.force_render = 0;
			render			 = 1;
		} else if(ev.type == ButtonPress) {
			MwLLMouse p;
			p.point.x = ev.xbutton.x;
			p.point.y = ev.xbutton.y;
			if(ev.xbutton.button == Button1) {
				p.button = MwLLMouseLeft;
			} else if(ev.xbutton.button == Button2) {
				p.button = MwLLMouseMiddle;
			} else if(ev.xbutton.button == Button3) {
				p.button = MwLLMouseRight;
			} else if(ev.xbutton.button == Button4) {
				p.button = MwLLMouseWheelUp;
			} else if(ev.xbutton.button == Button5) {
				p.button = MwLLMouseWheelDown;
			}

			XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);

			MwLLDispatch(handle, down, &p);
		} else if(ev.type == ButtonRelease) {
			MwLLMouse p;
			p.point.x = ev.xbutton.x;
			p.point.y = ev.xbutton.y;
			if(ev.xbutton.button == Button1) {
				p.button = MwLLMouseLeft;
			} else if(ev.xbutton.button == Button2) {
				p.button = MwLLMouseMiddle;
			} else if(ev.xbutton.button == Button3) {
				p.button = MwLLMouseRight;
			} else if(ev.xbutton.button == Button4) {
				p.button = MwLLMouseWheelUp;
			} else if(ev.xbutton.button == Button5) {
				p.button = MwLLMouseWheelDown;
			}

			MwLLDispatch(handle, up, &p);
		} else if(ev.type == ConfigureNotify) {
			if(handle->x11.width != (unsigned int)ev.xconfigure.width || handle->x11.height != (unsigned int)ev.xconfigure.height) {
				MwLLDispatch(handle, resize, NULL);
				destroy_pixmap(handle);
				create_pixmap(handle);
				render = 1;
			}
			handle->x11.width  = ev.xconfigure.width;
			handle->x11.height = ev.xconfigure.height;
		} else if(ev.type == ClientMessage) {
			if(ev.xclient.message_type == handle->x11.wm_protocols && ev.xclient.data.l[0] == (long)handle->x11.wm_delete) {
				MwLLDispatch(handle, close, NULL);
			}
		} else if(ev.type == FocusIn) {
			MwLLDispatch(handle, focus_in, NULL);
		} else if(ev.type == FocusOut) {
			MwLLDispatch(handle, focus_out, NULL);
		} else if(ev.type == MotionNotify) {
			MwPoint		  p;
			XWindowAttributes attr;

			XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

			p.x = ev.xmotion.x;
			p.y = ev.xmotion.y;

			if(handle->x11.grabbed) {
				p.x -= attr.width / 2;
				p.y -= attr.height / 2;
			}

			MwLLDispatch(handle, move, &p);
			if(handle->x11.grabbed && (p.x != 0 || p.y != 0)) {
				XWarpPointer(handle->x11.display, None, handle->x11.window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
			}
		} else if(ev.type == KeyPress || ev.type == KeyRelease) {
			int    n = -1;
			char   str[512];
			KeySym sym;

			str[0] = 0;

			XLookupString(&ev.xkey, str, 512, &sym, NULL);

			/* wtf is wrong with xlib? */
			if(strlen(str) == 0 || (str[0] < 0x20)) {
				char* s = XKeysymToString(sym);

				strcpy(str, s);
			}

			if(MwStringIsKeyUTF8(sym)) {
				char s = str[0];

				if(ev.xkey.state & (ShiftMask | LockMask) && !(ev.xkey.state & (ControlMask | Mod1Mask))) {
					n = toupper((int)s);
				} else {
					n = s;
				}
			} else if(strcmp(str, "BackSpace") == 0) {
				n = MwLLKeyBackSpace;
			} else if(strcmp(str, "Left") == 0) {
				n = MwLLKeyLeft;
			} else if(strcmp(str, "Right") == 0) {
				n = MwLLKeyRight;
			} else if(strcmp(str, "Up") == 0) {
				n = MwLLKeyUp;
			} else if(strcmp(str, "Down") == 0) {
				n = MwLLKeyDown;
			} else if(strcmp(str, "Return") == 0) {
				n = MwLLKeyEnter;
			} else if(strcmp(str, "Escape") == 0) {
				n = MwLLKeyEscape;
			} else if(strcmp(str, "Shift_L") == 0) {
				n = MwLLKeyLeftShift;
			} else if(strcmp(str, "Shift_R") == 0) {
				n = MwLLKeyRightShift;
			} else if(strcmp(str, "Alt_L") == 0 || strcmp(str, "Alt_R") == 0) {
				n = MwLLKeyAlt;
			} else if(strcmp(str, "Control_R") == 0 || strcmp(str, "Control_R") == 0) {
				n = MwLLKeyControl;
			}

			if(n != MwLLKeyControl && ev.xkey.state & ControlMask) {
				n |= MwLLControlMask;
			}
			if(n != MwLLKeyAlt && ev.xkey.state & Mod1Mask) {
				n |= MwLLAltMask;
			}

			if(n != -1) {
				if(ev.type == KeyPress) {
					MwLLDispatch(handle, key, &n);
				} else {
					MwLLDispatch(handle, key_released, &n);
				}
			}
		} else if(ev.type == SelectionNotify) {
			handle->x11.clipboard_pending = 0;

			if(ev.xselection.property != None) {
				Atom	       type;
				int	       format;
				unsigned long  nitems, after, size;
				unsigned char* pdat;
				int	       ret = XGetWindowProperty(handle->x11.display, handle->x11.window, handle->x11.selection, 0, 0, False, AnyPropertyType, &type, &format, &nitems, &after, &pdat);

				if(pdat != NULL) XFree(pdat);

				size = after;

				if(ret == Success && type != None && format == 8) {
					char* buf = malloc(size + 1);
					int   offset;

					for(offset = 0; after > 0; offset += nitems) {
						ret = XGetWindowProperty(handle->x11.display, handle->x11.window, handle->x11.selection, offset / 4, after / 4 + 1, False, AnyPropertyType, &type, &format, &nitems, &after, &pdat);

						if(ret != Success) {
							free(buf);
							buf = NULL;
							break;
						}

						memcpy(buf + offset, pdat, nitems);

						XFree(pdat);
					}

					if(buf != NULL) {
						buf[size] = 0;

						MwLLDispatch(handle, clipboard, buf);

						free(buf);
					}
				}

				XDeleteProperty(handle->x11.display, handle->x11.window, handle->x11.selection);
			}
		}
		if(render) {
			int	     x, y;
			unsigned int w, h;

			MwLLGetXYWH(handle, &x, &y, &w, &h);

			MwLLDispatch(handle, draw, NULL);
			if(handle->common.copy_buffer) {
				XCopyArea(handle->x11.display, handle->x11.pixmap, handle->x11.window, handle->x11.gc, 0, 0, w, h, 0, 0);
				XSetWindowBackgroundPixmap(handle->x11.display, handle->x11.window, handle->x11.pixmap);
			}
		}
	}
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	XSetStandardProperties(handle->x11.display, handle->x11.window, title, title, None, (char**)NULL, 0, NULL);
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap	  r = malloc(sizeof(*r));
	int		  evbase, erbase;
	XWindowAttributes attr;

	r->common.raw = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	r->common.width	 = width;
	r->common.height = height;

	r->x11.depth   = attr.depth;
	r->x11.display = handle->x11.display;
	r->x11.data    = malloc(sizeof(unsigned long) * width * height);
	r->x11.handle  = handle;

#ifdef USE_XRENDER
	r->x11.use_xrender = XRenderQueryExtension(handle->x11.display, &evbase, &erbase) ? 1 : 0;
#endif

	r->x11.image = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), r->x11.depth, ZPixmap, 0, NULL, width, height, 32, 0);
	r->x11.mask  = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, NULL, width, height, 32, 0);

	r->x11.image->data = malloc(r->x11.image->bytes_per_line * height);
	r->x11.mask->data  = malloc(r->x11.mask->bytes_per_line * height);

	MwLLPixmapUpdate(r);
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	int y, x;
	for(y = 0; y < r->common.height; y++) {
		for(x = 0; x < r->common.width; x++) {
			unsigned char* px = &r->common.raw[(y * r->common.width + x) * 4];
			MwLLColor      c  = NULL;
			unsigned long  p;

			c = MwLLAllocColor(r->x11.handle, px[0], px[1], px[2]);
			p = c->x11.pixel;
			MwLLFreeColor(c);

			XPutPixel(r->x11.image, x, y, p);
			*(unsigned long*)(&r->x11.data[(y * r->common.width + x) * sizeof(unsigned long)]) = (px[3] << 24) | p;
		}
	}

	for(y = 0; y < r->common.height; y++) {
		for(x = 0; x < r->common.width; x++) {
			if(r->common.raw[(y * r->common.width + x) * 4 + 3]) {
				XPutPixel(r->x11.mask, x, y, 1);
			} else {
				XPutPixel(r->x11.mask, x, y, 0);
			}
		}
	}
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap->common.raw);
	XDestroyImage(pixmap->x11.image);
	XDestroyImage(pixmap->x11.mask);
	free(pixmap->x11.data);

	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	if(rect->width <= 0 || rect->height <= 0 || pixmap->common.width <= 0 || pixmap->common.height <= 0) return;
#ifdef USE_XRENDER
	if(pixmap->x11.image != NULL && pixmap->x11.use_xrender) {
		Pixmap			 px	= XCreatePixmap(handle->x11.display, handle->x11.window, pixmap->common.width, pixmap->common.height, pixmap->x11.depth);
		Pixmap			 mask	= XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, 1);
		Pixmap			 pxsrc	= XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, pixmap->x11.depth);
		GC			 maskgc = XCreateGC(handle->x11.display, mask, 0, NULL);
		XRenderPictFormat*	 format = XRenderFindStandardFormat(handle->x11.display, PictStandardRGB24);
		XRenderPictureAttributes attr;
		Picture			 src, dest;
		XTransform		 m;
		double			 xsc = (double)pixmap->common.width / rect->width;
		double			 ysc = (double)pixmap->common.height / rect->height;
		char*			 dm  = malloc(rect->width * rect->height * 4);
		XImage*			 destmask;
		int			 y, x;

		destmask = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int sy = y * pixmap->common.height / rect->height;
				int sx = x * pixmap->common.width / rect->width;
				sy     = (int)sy;
				sx     = (int)sx;

				XPutPixel(destmask, x, y, XGetPixel(pixmap->x11.mask, sx, sy));
			}
		}

		XPutImage(handle->x11.display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

		m.matrix[0][0] = XDoubleToFixed(xsc);
		m.matrix[0][1] = XDoubleToFixed(0);
		m.matrix[0][2] = XDoubleToFixed(0);

		m.matrix[1][0] = XDoubleToFixed(0);
		m.matrix[1][1] = XDoubleToFixed(ysc);
		m.matrix[1][2] = XDoubleToFixed(0);

		m.matrix[2][0] = XDoubleToFixed(0);
		m.matrix[2][1] = XDoubleToFixed(0);
		m.matrix[2][2] = XDoubleToFixed(1.0);

		memset(&attr, 0, sizeof(attr));

		XPutImage(handle->x11.display, px, handle->x11.gc, pixmap->x11.image, 0, 0, 0, 0, pixmap->common.width, pixmap->common.height);

		src  = XRenderCreatePicture(handle->x11.display, px, format, 0, &attr);
		dest = XRenderCreatePicture(handle->x11.display, pxsrc, format, 0, &attr);

		XRenderSetPictureTransform(handle->x11.display, src, &m);
		XRenderComposite(handle->x11.display, PictOpSrc, src, 0, dest, 0, 0, 0, 0, 0, 0, rect->width, rect->height);

		XRenderFreePicture(handle->x11.display, src);
		XRenderFreePicture(handle->x11.display, dest);

		XSetClipMask(handle->x11.display, handle->x11.gc, mask);
		XSetClipOrigin(handle->x11.display, handle->x11.gc, rect->x, rect->y);
		XCopyArea(handle->x11.display, pxsrc, handle->x11.pixmap, handle->x11.gc, 0, 0, rect->width, rect->height, rect->x, rect->y);
		XSetClipMask(handle->x11.display, handle->x11.gc, None);

		XDestroyImage(destmask);

		XFreeGC(handle->x11.display, maskgc);
		XFreePixmap(handle->x11.display, mask);
		XFreePixmap(handle->x11.display, px);
		XFreePixmap(handle->x11.display, pxsrc);
	} else
#endif
	    if(pixmap->x11.image != NULL) {
		XImage* dest;
		XImage* destmask;
		Pixmap	mask   = XCreatePixmap(handle->x11.display, handle->x11.window, rect->width, rect->height, 1);
		GC	maskgc = XCreateGC(handle->x11.display, mask, 0, NULL);
		char*	di     = malloc(rect->width * rect->height * 4);
		char*	dm     = malloc(rect->width * rect->height * 4);
		int	y, x;

		dest	 = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), pixmap->x11.depth, ZPixmap, 0, di, rect->width, rect->height, 32, rect->width * 4);
		destmask = XCreateImage(handle->x11.display, DefaultVisual(handle->x11.display, DefaultScreen(handle->x11.display)), 1, ZPixmap, 0, dm, rect->width, rect->height, 32, rect->width * 4);

		for(y = 0; y < (int)rect->height; y++) {
			for(x = 0; x < (int)rect->width; x++) {
				int   sy = y * pixmap->common.height / rect->height;
				int   sx = x * pixmap->common.width / rect->width;
				char* ipx;
				char* opx;
				sy = (int)sy;
				sx = (int)sx;

				ipx = &pixmap->x11.image->data[(pixmap->common.width * sy + sx) * (pixmap->x11.image->bitmap_unit / 8)];
				opx = &di[(rect->width * y + x) * (pixmap->x11.image->bitmap_unit / 8)];
				memcpy(opx, ipx, pixmap->x11.image->bitmap_unit / 8);

				XPutPixel(destmask, x, y, XGetPixel(pixmap->x11.mask, sx, sy));
			}
		}

		XPutImage(handle->x11.display, mask, maskgc, destmask, 0, 0, 0, 0, rect->width, rect->height);

		XSetClipMask(handle->x11.display, handle->x11.gc, mask);
		XSetClipOrigin(handle->x11.display, handle->x11.gc, rect->x, rect->y);
		XPutImage(handle->x11.display, handle->x11.pixmap, handle->x11.gc, dest, 0, 0, rect->x, rect->y, rect->width, rect->height);
		XSetClipMask(handle->x11.display, handle->x11.gc, None);

		XDestroyImage(dest);
		XDestroyImage(destmask);

		XFreeGC(handle->x11.display, maskgc);
		XFreePixmap(handle->x11.display, mask);
	}
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	unsigned long* icon = malloc((2 + pixmap->common.width * pixmap->common.height) * sizeof(*icon));
	int	       i;
	Atom	       atom = XInternAtom(handle->x11.display, "_NET_WM_ICON", False);

	icon[0] = pixmap->common.width;
	icon[1] = pixmap->common.height;

	for(i = 0; i < pixmap->common.width * pixmap->common.height; i++) {
		icon[2 + i] = *(unsigned long*)(&pixmap->x11.data[i * sizeof(unsigned long)]);
	}

	XChangeProperty(handle->x11.display, handle->x11.window, atom, 6, 32, PropModeReplace, (unsigned char*)icon, 2 + pixmap->common.width * pixmap->common.height);

	free(icon);
}

static void MwLLForceRenderImpl(MwLL handle) {
	if(!handle->x11.force_render) {
		XEvent ev;
		memset(&ev, 0, sizeof(ev));

		ev.type		  = Expose;
		ev.xexpose.window = handle->x11.window;
		XSendEvent(handle->x11.display, handle->x11.window, False, ExposureMask, &ev);

		handle->x11.force_render = 1;
	}
}

Cursor MwLLX11CreateCursor(Display* display, MwCursor* image, MwCursor* mask) {
	Cursor	cur;
	int	y, x, ys, xs;
	char*	di	= malloc(MwCursorDataHeight * MwCursorDataHeight * 4);
	char*	dm	= malloc(MwCursorDataHeight * MwCursorDataHeight * 4);
	XImage* cimage	= XCreateImage(display, DefaultVisual(display, DefaultScreen(display)), 1, ZPixmap, 0, di, MwCursorDataHeight, MwCursorDataHeight, 32, MwCursorDataHeight * 4);
	XImage* cmask	= XCreateImage(display, DefaultVisual(display, DefaultScreen(display)), 1, ZPixmap, 0, dm, MwCursorDataHeight, MwCursorDataHeight, 32, MwCursorDataHeight * 4);
	Pixmap	pimage	= XCreatePixmap(display, DefaultRootWindow(display), MwCursorDataHeight, MwCursorDataHeight, 1);
	Pixmap	pmask	= XCreatePixmap(display, DefaultRootWindow(display), MwCursorDataHeight, MwCursorDataHeight, 1);
	GC	imagegc = XCreateGC(display, pimage, 0, NULL);
	GC	maskgc	= XCreateGC(display, pmask, 0, NULL);
	XColor	cfg, cbg;

	xs = -mask->x + image->x;
	ys = mask->height + mask->y;
	ys = image->height + image->y - ys;

	memset(cimage->data, 0, cimage->bytes_per_line * cimage->height);
	memset(cmask->data, 0, cmask->bytes_per_line * cmask->height);
	for(y = 0; y < mask->height; y++) {
		unsigned int l = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			if(l & 1) {
				XPutPixel(cmask, x, y, 1);
			}
			l = l >> 1;
		}
	}
	for(y = 0; y < image->height; y++) {
		unsigned int l = image->data[y];
		for(x = image->width - 1; x >= 0; x--) {
			int px = 0;
			if(l & 1) px = 1;
			XPutPixel(cimage, xs + x, ys + y, px);

			l = l >> 1;
		}
	}

	cfg.red	  = 65535;
	cfg.green = 65535;
	cfg.blue  = 65535;
	XAllocColor(display, DefaultColormap(display, DefaultScreen(display)), &cfg);

	cbg.red	  = 0;
	cbg.green = 0;
	cbg.blue  = 0;
	XAllocColor(display, DefaultColormap(display, DefaultScreen(display)), &cbg);

	XPutImage(display, pimage, imagegc, cimage, 0, 0, 0, 0, MwCursorDataHeight, MwCursorDataHeight);
	XPutImage(display, pmask, maskgc, cmask, 0, 0, 0, 0, MwCursorDataHeight, MwCursorDataHeight);

	cur = XCreatePixmapCursor(display, pimage, pmask, &cfg, &cbg, xs - image->x, ys + (MwCursorDataHeight + image->y));

	XFreePixmap(display, pimage);
	XFreePixmap(display, pmask);

	XDestroyImage(cimage);
	XDestroyImage(cmask);

	return cur;
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	Cursor cur = MwLLX11CreateCursor(handle->x11.display, image, mask);

	XDefineCursor(handle->x11.display, handle->x11.window, cur);
	XFreeCursor(handle->x11.display, cur);
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	int		  x = 0, y = 0;
	Window		  child, root, parent;
	Window*		  children;
	unsigned int	  nchild;
	XWindowAttributes xwa;

	handle->x11.top = 1;

	XQueryTree(handle->x11.display, handle->x11.window, &root, &parent, &children, &nchild);
	if(children != NULL) XFree(children);

	XTranslateCoordinates(handle->x11.display, parent, RootWindow(handle->x11.display, DefaultScreen(handle->x11.display)), 0, 0, &x, &y, &child);

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &xwa);

	XReparentWindow(handle->x11.display, handle->x11.window, RootWindow(handle->x11.display, DefaultScreen(handle->x11.display)), x + point->x, y + point->y);

	if(xwa.map_state == IsViewable) {
		sync_move(handle, x + point->x, y + point->y);
	}
}

static void MwLLShowImpl(MwLL handle, int show) {
	if(show) {
		wait_map(handle, 1);

		XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);
	} else {
		wait_unmap(handle, 1);
	}
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	Atom wndtype  = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE", False);
	Atom wnddlg   = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	Atom wndstate = XInternAtom(handle->x11.display, "_NET_WM_STATE", False);
	Atom wndmodal = XInternAtom(handle->x11.display, "_NET_WM_STATE_MODAL", False);

	if(parent != NULL) XSetTransientForHint(handle->x11.display, handle->x11.window, parent->x11.window);
	XChangeProperty(handle->x11.display, handle->x11.window, wndtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wnddlg, 1);
	XChangeProperty(handle->x11.display, handle->x11.window, wndstate, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wndmodal, 1);
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	XSizeHints* hints = XAllocSizeHints();
	long	    ret;

	XGetWMSizeHints(handle->x11.display, handle->x11.window, hints, &ret, XA_WM_NORMAL_HINTS);

	hints->flags |= PMinSize | PMaxSize;
	hints->min_width  = minx;
	hints->min_height = miny;
	hints->max_width  = maxx;
	hints->max_height = maxy;
	XSetWMSizeHints(handle->x11.display, handle->x11.window, hints, XA_WM_NORMAL_HINTS);
	XFree(hints);
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	Atom	    atom = XInternAtom(handle->x11.display, "_MOTIF_WM_HINTS", 0);
	mwm_hints_t hints;

	hints.flags	  = MWM_HINTS_DECORATIONS;
	hints.decorations = toggle ? 0 : 1;
	XChangeProperty(handle->x11.display, handle->x11.window, atom, atom, 32, PropModeReplace, (unsigned char*)&hints, 5);
}

static void MwLLFocusImpl(MwLL handle) {
	XSetInputFocus(handle->x11.display, handle->x11.window, RevertToNone, CurrentTime);
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	XWindowAttributes attr;

	XGetWindowAttributes(handle->x11.display, handle->x11.window, &attr);

	if(toggle) {
		handle->x11.grabbed = 1;

		XWarpPointer(handle->x11.display, None, handle->x11.window, 0, 0, 0, 0, attr.width / 2, attr.height / 2);
	} else {
		handle->x11.grabbed = 0;
	}
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	/* TODO */

	(void)handle;
	(void)text;
}

static void MwLLGetClipboardImpl(MwLL handle) {
	if(handle->x11.clipboard_pending) return;

	XConvertSelection(handle->x11.display, handle->x11.clipboard, handle->x11.utf8_string, handle->x11.selection, handle->x11.window, CurrentTime);

	handle->x11.clipboard_pending = 1;
	handle->x11.clipboard_time    = MwTimeGetTick();
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	XSetWindowAttributes xswa;
	Atom		     wndtype = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE", False);
	Atom		     wndmenu = XInternAtom(handle->x11.display, "_NET_WM_WINDOW_TYPE_MENU", False);

	xswa.override_redirect = True;

	XChangeWindowAttributes(handle->x11.display, handle->x11.window, CWOverrideRedirect, &xswa);
	XChangeProperty(handle->x11.display, handle->x11.window, wndtype, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wndmenu, 1);
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	Window	     root, child;
	int	     rx, ry, wx, wy;
	unsigned int m;

	XQueryPointer(handle->x11.display, DefaultRootWindow(handle->x11.display), &root, &child, &rx, &ry, &wx, &wy, &m);

	point->x = rx;
	point->y = ry;
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	XWindowAttributes xwa;
	XGetWindowAttributes(handle->x11.display, DefaultRootWindow(handle->x11.display), &xwa);

	rect->x = rect->y = 0;
	rect->width	  = xwa.width;
	rect->height	  = xwa.height;
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static int MwLLX11CallInitImpl(void) {
	xsymtbl.lib_xlib = MwDynamicOpen("libX11.so");

#if USE_XRENDER
	xsymtbl.lib_xrender = MwDynamicOpen("libXrender.so");
#endif

#define X11_FUNC_LOAD(x) x = MwDynamicSymbol(xsymtbl.lib_xlib, #x)

	X11_FUNC_LOAD(XLoadQueryFont);
	X11_FUNC_LOAD(XQueryFont);
	X11_FUNC_LOAD(XGetMotionEvents);
	X11_FUNC_LOAD(XDeleteModifiermapEntry);
	X11_FUNC_LOAD(XGetModifierMapping);
	X11_FUNC_LOAD(XInsertModifiermapEntry);
	X11_FUNC_LOAD(XNewModifiermap);
	X11_FUNC_LOAD(XCreateImage);
	X11_FUNC_LOAD(XInitImage);
	X11_FUNC_LOAD(XGetImage);
	X11_FUNC_LOAD(XGetSubImage);
	X11_FUNC_LOAD(XOpenDisplay);
	X11_FUNC_LOAD(XrmInitialize);
	X11_FUNC_LOAD(XFetchBytes);
	X11_FUNC_LOAD(XFetchBuffer);
	X11_FUNC_LOAD(XGetAtomName);
	X11_FUNC_LOAD(XGetAtomNames);
	X11_FUNC_LOAD(XGetDefault);
	X11_FUNC_LOAD(XDisplayName);
	X11_FUNC_LOAD(XKeysymToString);
	X11_FUNC_LOAD(XInternAtom);
	X11_FUNC_LOAD(XInternAtoms);
	X11_FUNC_LOAD(XCopyColormapAndFree);
	X11_FUNC_LOAD(XCreateColormap);
	X11_FUNC_LOAD(XCreatePixmapCursor);
	X11_FUNC_LOAD(XCreateGlyphCursor);
	X11_FUNC_LOAD(XCreateFontCursor);
	X11_FUNC_LOAD(XLoadFont);
	X11_FUNC_LOAD(XCreateGC);
	X11_FUNC_LOAD(XGContextFromGC);
	X11_FUNC_LOAD(XFlushGC);
	X11_FUNC_LOAD(XCreatePixmap);
	X11_FUNC_LOAD(XCreateBitmapFromData);
	X11_FUNC_LOAD(XCreatePixmapFromBitmapData);
	X11_FUNC_LOAD(XCreateSimpleWindow);
	X11_FUNC_LOAD(XGetSelectionOwner);
	X11_FUNC_LOAD(XCreateWindow);
	X11_FUNC_LOAD(XListInstalledColormaps);
	X11_FUNC_LOAD(XListFonts);
	X11_FUNC_LOAD(XListFontsWithInfo);
	X11_FUNC_LOAD(XGetFontPath);
	X11_FUNC_LOAD(XListExtensions);
	X11_FUNC_LOAD(XListProperties);
	X11_FUNC_LOAD(XListHosts);
	X11_FUNC_LOAD(XKeycodeToKeysym);
	X11_FUNC_LOAD(XLookupKeysym);
	X11_FUNC_LOAD(XGetKeyboardMapping);
	X11_FUNC_LOAD(XStringToKeysym);
	X11_FUNC_LOAD(XMaxRequestSize);
	X11_FUNC_LOAD(XExtendedMaxRequestSize);
	X11_FUNC_LOAD(XResourceManagerString);
	X11_FUNC_LOAD(XScreenResourceString);
	X11_FUNC_LOAD(XDisplayMotionBufferSize);
	X11_FUNC_LOAD(XVisualIDFromVisual);
	X11_FUNC_LOAD(XInitThreads);
	X11_FUNC_LOAD(XFreeThreads);
	X11_FUNC_LOAD(XLockDisplay);
	X11_FUNC_LOAD(XUnlockDisplay);
	X11_FUNC_LOAD(XInitExtension);
	X11_FUNC_LOAD(XAddExtension);
	X11_FUNC_LOAD(XFindOnExtensionList);
	X11_FUNC_LOAD(XEHeadOfExtensionList);
	X11_FUNC_LOAD(XRootWindow);
	X11_FUNC_LOAD(XDefaultRootWindow);
	X11_FUNC_LOAD(XRootWindowOfScreen);
	X11_FUNC_LOAD(XDefaultVisual);
	X11_FUNC_LOAD(XDefaultVisualOfScreen);
	X11_FUNC_LOAD(XDefaultGC);
	X11_FUNC_LOAD(XDefaultGCOfScreen);
	X11_FUNC_LOAD(XBlackPixel);
	X11_FUNC_LOAD(XWhitePixel);
	X11_FUNC_LOAD(XAllPlanes);
	X11_FUNC_LOAD(XBlackPixelOfScreen);
	X11_FUNC_LOAD(XWhitePixelOfScreen);
	X11_FUNC_LOAD(XNextRequest);
	X11_FUNC_LOAD(XLastKnownRequestProcessed);
	X11_FUNC_LOAD(XServerVendor);
	X11_FUNC_LOAD(XDisplayString);
	X11_FUNC_LOAD(XDefaultColormap);
	X11_FUNC_LOAD(XDefaultColormapOfScreen);
	X11_FUNC_LOAD(XDisplayOfScreen);
	X11_FUNC_LOAD(XScreenOfDisplay);
	X11_FUNC_LOAD(XDefaultScreenOfDisplay);
	X11_FUNC_LOAD(XEventMaskOfScreen);
	X11_FUNC_LOAD(XScreenNumberOfScreen);
	X11_FUNC_LOAD(XSetErrorHandler);
	X11_FUNC_LOAD(XSetIOErrorHandler);
	X11_FUNC_LOAD(XSetIOErrorExitHandler);
	X11_FUNC_LOAD(XListPixmapFormats);
	X11_FUNC_LOAD(XListDepths);
	X11_FUNC_LOAD(XReconfigureWMWindow);
	X11_FUNC_LOAD(XGetWMProtocols);
	X11_FUNC_LOAD(XSetWMProtocols);
	X11_FUNC_LOAD(XIconifyWindow);
	X11_FUNC_LOAD(XWithdrawWindow);
	X11_FUNC_LOAD(XGetCommand);
	X11_FUNC_LOAD(XGetWMColormapWindows);
	X11_FUNC_LOAD(XSetWMColormapWindows);
	X11_FUNC_LOAD(XFreeStringList);
	X11_FUNC_LOAD(XSetTransientForHint);
	X11_FUNC_LOAD(XActivateScreenSaver);
	X11_FUNC_LOAD(XAddHost);
	X11_FUNC_LOAD(XAddHosts);
	X11_FUNC_LOAD(XAddToExtensionList);
	X11_FUNC_LOAD(XAddToSaveSet);
	X11_FUNC_LOAD(XAllocColor);
	X11_FUNC_LOAD(XAllocColorCells);
	X11_FUNC_LOAD(XAllocColorPlanes);
	X11_FUNC_LOAD(XAllocNamedColor);
	X11_FUNC_LOAD(XAllowEvents);
	X11_FUNC_LOAD(XAutoRepeatOff);
	X11_FUNC_LOAD(XAutoRepeatOn);
	X11_FUNC_LOAD(XBell);
	X11_FUNC_LOAD(XBitmapBitOrder);
	X11_FUNC_LOAD(XBitmapPad);
	X11_FUNC_LOAD(XBitmapUnit);
	X11_FUNC_LOAD(XCellsOfScreen);
	X11_FUNC_LOAD(XChangeActivePointerGrab);
	X11_FUNC_LOAD(XChangeGC);
	X11_FUNC_LOAD(XChangeKeyboardControl);
	X11_FUNC_LOAD(XChangeKeyboardMapping);
	X11_FUNC_LOAD(XChangePointerControl);
	X11_FUNC_LOAD(XChangeProperty);
	X11_FUNC_LOAD(XChangeSaveSet);
	X11_FUNC_LOAD(XChangeWindowAttributes);
	X11_FUNC_LOAD(XCheckIfEvent);
	X11_FUNC_LOAD(XCheckMaskEvent);
	X11_FUNC_LOAD(XCheckTypedEvent);
	X11_FUNC_LOAD(XCheckTypedWindowEvent);
	X11_FUNC_LOAD(XCheckWindowEvent);
	X11_FUNC_LOAD(XCirculateSubwindows);
	X11_FUNC_LOAD(XCirculateSubwindowsDown);
	X11_FUNC_LOAD(XCirculateSubwindowsUp);
	X11_FUNC_LOAD(XClearArea);
	X11_FUNC_LOAD(XClearWindow);
	X11_FUNC_LOAD(XCloseDisplay);
	X11_FUNC_LOAD(XConfigureWindow);
	X11_FUNC_LOAD(XConnectionNumber);
	X11_FUNC_LOAD(XConvertSelection);
	X11_FUNC_LOAD(XCopyArea);
	X11_FUNC_LOAD(XCopyGC);
	X11_FUNC_LOAD(XCopyPlane);
	X11_FUNC_LOAD(XDefaultDepth);
	X11_FUNC_LOAD(XDefaultDepthOfScreen);
	X11_FUNC_LOAD(XDefaultScreen);
	X11_FUNC_LOAD(XDefineCursor);
	X11_FUNC_LOAD(XDeleteProperty);
	X11_FUNC_LOAD(XDestroyWindow);
	X11_FUNC_LOAD(XDestroySubwindows);
	X11_FUNC_LOAD(XDoesBackingStore);
	X11_FUNC_LOAD(XDoesSaveUnders);
	X11_FUNC_LOAD(XDisableAccessControl);
	X11_FUNC_LOAD(XDisplayCells);
	X11_FUNC_LOAD(XDisplayHeight);
	X11_FUNC_LOAD(XDisplayHeightMM);
	X11_FUNC_LOAD(XDisplayKeycodes);
	X11_FUNC_LOAD(XDisplayPlanes);
	X11_FUNC_LOAD(XDisplayWidth);
	X11_FUNC_LOAD(XDisplayWidthMM);
	X11_FUNC_LOAD(XDrawArc);
	X11_FUNC_LOAD(XDrawArcs);
	X11_FUNC_LOAD(XDrawImageString);
	X11_FUNC_LOAD(XDrawImageString16);
	X11_FUNC_LOAD(XDrawLine);
	X11_FUNC_LOAD(XDrawLines);
	X11_FUNC_LOAD(XDrawPoint);
	X11_FUNC_LOAD(XDrawPoints);
	X11_FUNC_LOAD(XDrawRectangle);
	X11_FUNC_LOAD(XDrawRectangles);
	X11_FUNC_LOAD(XDrawSegments);
	X11_FUNC_LOAD(XDrawString);
	X11_FUNC_LOAD(XDrawString16);
	X11_FUNC_LOAD(XDrawText);
	X11_FUNC_LOAD(XDrawText16);
	X11_FUNC_LOAD(XEnableAccessControl);
	X11_FUNC_LOAD(XEventsQueued);
	X11_FUNC_LOAD(XFetchName);
	X11_FUNC_LOAD(XFillArc);
	X11_FUNC_LOAD(XFillArcs);
	X11_FUNC_LOAD(XFillPolygon);
	X11_FUNC_LOAD(XFillRectangle);
	X11_FUNC_LOAD(XFillRectangles);
	X11_FUNC_LOAD(XFlush);
	X11_FUNC_LOAD(XForceScreenSaver);
	X11_FUNC_LOAD(XFree);
	X11_FUNC_LOAD(XFreeColormap);
	X11_FUNC_LOAD(XFreeColors);
	X11_FUNC_LOAD(XFreeCursor);
	X11_FUNC_LOAD(XFreeExtensionList);
	X11_FUNC_LOAD(XFreeFont);
	X11_FUNC_LOAD(XFreeFontInfo);
	X11_FUNC_LOAD(XFreeFontNames);
	X11_FUNC_LOAD(XFreeFontPath);
	X11_FUNC_LOAD(XFreeGC);
	X11_FUNC_LOAD(XFreeModifiermap);
	X11_FUNC_LOAD(XFreePixmap);
	X11_FUNC_LOAD(XGeometry);
	X11_FUNC_LOAD(XGetErrorDatabaseText);
	X11_FUNC_LOAD(XGetErrorText);
	X11_FUNC_LOAD(XGetFontProperty);
	X11_FUNC_LOAD(XGetGCValues);
	X11_FUNC_LOAD(XGetGeometry);
	X11_FUNC_LOAD(XGetIconName);
	X11_FUNC_LOAD(XGetInputFocus);
	X11_FUNC_LOAD(XGetKeyboardControl);
	X11_FUNC_LOAD(XGetPointerControl);
	X11_FUNC_LOAD(XGetPointerMapping);
	X11_FUNC_LOAD(XGetScreenSaver);
	X11_FUNC_LOAD(XGetTransientForHint);
	X11_FUNC_LOAD(XGetWindowProperty);
	X11_FUNC_LOAD(XGetWindowAttributes);
	X11_FUNC_LOAD(XGrabButton);
	X11_FUNC_LOAD(XGrabKey);
	X11_FUNC_LOAD(XGrabKeyboard);
	X11_FUNC_LOAD(XGrabPointer);
	X11_FUNC_LOAD(XGrabServer);
	X11_FUNC_LOAD(XHeightMMOfScreen);
	X11_FUNC_LOAD(XHeightOfScreen);
	X11_FUNC_LOAD(XIfEvent);
	X11_FUNC_LOAD(XImageByteOrder);
	X11_FUNC_LOAD(XInstallColormap);
	X11_FUNC_LOAD(XKeysymToKeycode);
	X11_FUNC_LOAD(XKillClient);
	X11_FUNC_LOAD(XLookupColor);
	X11_FUNC_LOAD(XLowerWindow);
	X11_FUNC_LOAD(XMapRaised);
	X11_FUNC_LOAD(XMapSubwindows);
	X11_FUNC_LOAD(XMapWindow);
	X11_FUNC_LOAD(XMaskEvent);
	X11_FUNC_LOAD(XMaxCmapsOfScreen);
	X11_FUNC_LOAD(XMinCmapsOfScreen);
	X11_FUNC_LOAD(XMoveResizeWindow);
	X11_FUNC_LOAD(XMoveWindow);
	X11_FUNC_LOAD(XNextEvent);
	X11_FUNC_LOAD(XNoOp);
	X11_FUNC_LOAD(XParseColor);
	X11_FUNC_LOAD(XParseGeometry);
	X11_FUNC_LOAD(XPeekEvent);
	X11_FUNC_LOAD(XPeekIfEvent);
	X11_FUNC_LOAD(XPending);
	X11_FUNC_LOAD(XPlanesOfScreen);
	X11_FUNC_LOAD(XProtocolRevision);
	X11_FUNC_LOAD(XProtocolVersion);
	X11_FUNC_LOAD(XPutBackEvent);
	X11_FUNC_LOAD(XPutImage);
	X11_FUNC_LOAD(XQLength);
	X11_FUNC_LOAD(XQueryBestCursor);
	X11_FUNC_LOAD(XQueryBestSize);
	X11_FUNC_LOAD(XQueryBestStipple);
	X11_FUNC_LOAD(XQueryBestTile);
	X11_FUNC_LOAD(XQueryColor);
	X11_FUNC_LOAD(XQueryColors);
	X11_FUNC_LOAD(XQueryExtension);
	X11_FUNC_LOAD(XQueryKeymap);
	X11_FUNC_LOAD(XQueryPointer);
	X11_FUNC_LOAD(XQueryTextExtents);
	X11_FUNC_LOAD(XQueryTextExtents16);
	X11_FUNC_LOAD(XQueryTree);
	X11_FUNC_LOAD(XRaiseWindow);
	X11_FUNC_LOAD(XReadBitmapFile);
	X11_FUNC_LOAD(XReadBitmapFileData);
	X11_FUNC_LOAD(XRebindKeysym);
	X11_FUNC_LOAD(XRecolorCursor);
	X11_FUNC_LOAD(XRefreshKeyboardMapping);
	X11_FUNC_LOAD(XRemoveFromSaveSet);
	X11_FUNC_LOAD(XRemoveHost);
	X11_FUNC_LOAD(XRemoveHosts);
	X11_FUNC_LOAD(XReparentWindow);
	X11_FUNC_LOAD(XResetScreenSaver);
	X11_FUNC_LOAD(XResizeWindow);
	X11_FUNC_LOAD(XRestackWindows);
	X11_FUNC_LOAD(XRotateBuffers);
	X11_FUNC_LOAD(XRotateWindowProperties);
	X11_FUNC_LOAD(XScreenCount);
	X11_FUNC_LOAD(XSelectInput);
	X11_FUNC_LOAD(XSendEvent);
	X11_FUNC_LOAD(XSetAccessControl);
	X11_FUNC_LOAD(XSetArcMode);
	X11_FUNC_LOAD(XSetBackground);
	X11_FUNC_LOAD(XSetClipMask);
	X11_FUNC_LOAD(XSetClipOrigin);
	X11_FUNC_LOAD(XSetClipRectangles);
	X11_FUNC_LOAD(XSetCloseDownMode);
	X11_FUNC_LOAD(XSetCommand);
	X11_FUNC_LOAD(XSetDashes);
	X11_FUNC_LOAD(XSetFillRule);
	X11_FUNC_LOAD(XSetFillStyle);
	X11_FUNC_LOAD(XSetFont);
	X11_FUNC_LOAD(XSetFontPath);
	X11_FUNC_LOAD(XSetForeground);
	X11_FUNC_LOAD(XSetFunction);
	X11_FUNC_LOAD(XSetGraphicsExposures);
	X11_FUNC_LOAD(XSetIconName);
	X11_FUNC_LOAD(XSetInputFocus);
	X11_FUNC_LOAD(XSetLineAttributes);
	X11_FUNC_LOAD(XSetModifierMapping);
	X11_FUNC_LOAD(XSetPlaneMask);
	X11_FUNC_LOAD(XSetPointerMapping);
	X11_FUNC_LOAD(XSetScreenSaver);
	X11_FUNC_LOAD(XSetSelectionOwner);
	X11_FUNC_LOAD(XSetState);
	X11_FUNC_LOAD(XSetStipple);
	X11_FUNC_LOAD(XSetSubwindowMode);
	X11_FUNC_LOAD(XSetTSOrigin);
	X11_FUNC_LOAD(XSetTile);
	X11_FUNC_LOAD(XSetWindowBackground);
	X11_FUNC_LOAD(XSetWindowBackgroundPixmap);
	X11_FUNC_LOAD(XSetWindowBorder);
	X11_FUNC_LOAD(XSetWindowBorderPixmap);
	X11_FUNC_LOAD(XSetWindowBorderWidth);
	X11_FUNC_LOAD(XSetWindowColormap);
	X11_FUNC_LOAD(XStoreBuffer);
	X11_FUNC_LOAD(XStoreBytes);
	X11_FUNC_LOAD(XStoreColor);
	X11_FUNC_LOAD(XStoreColors);
	X11_FUNC_LOAD(XStoreName);
	X11_FUNC_LOAD(XStoreNamedColor);
	X11_FUNC_LOAD(XSync);
	X11_FUNC_LOAD(XTextExtents);
	X11_FUNC_LOAD(XTextExtents16);
	X11_FUNC_LOAD(XTextWidth);
	X11_FUNC_LOAD(XTextWidth16);
	X11_FUNC_LOAD(XTranslateCoordinates);
	X11_FUNC_LOAD(XUndefineCursor);
	X11_FUNC_LOAD(XUngrabButton);
	X11_FUNC_LOAD(XUngrabKey);
	X11_FUNC_LOAD(XUngrabKeyboard);
	X11_FUNC_LOAD(XUngrabPointer);
	X11_FUNC_LOAD(XUngrabServer);
	X11_FUNC_LOAD(XUninstallColormap);
	X11_FUNC_LOAD(XUnloadFont);
	X11_FUNC_LOAD(XUnmapSubwindows);
	X11_FUNC_LOAD(XUnmapWindow);
	X11_FUNC_LOAD(XVendorRelease);
	X11_FUNC_LOAD(XWarpPointer);
	X11_FUNC_LOAD(XWidthMMOfScreen);
	X11_FUNC_LOAD(XWidthOfScreen);
	X11_FUNC_LOAD(XWindowEvent);
	X11_FUNC_LOAD(XWriteBitmapFile);
	X11_FUNC_LOAD(XSupportsLocale);
	X11_FUNC_LOAD(XSetLocaleModifiers);
	X11_FUNC_LOAD(XOpenOM);
	X11_FUNC_LOAD(XCloseOM);
	X11_FUNC_LOAD(XSetOMValues);
	X11_FUNC_LOAD(XGetOMValues);
	X11_FUNC_LOAD(XDisplayOfOM);
	X11_FUNC_LOAD(XLocaleOfOM);
	X11_FUNC_LOAD(XCreateOC);
	X11_FUNC_LOAD(XDestroyOC);
	X11_FUNC_LOAD(XOMOfOC);
	X11_FUNC_LOAD(XSetOCValues);
	X11_FUNC_LOAD(XGetOCValues);
	X11_FUNC_LOAD(XCreateFontSet);
	X11_FUNC_LOAD(XFreeFontSet);
	X11_FUNC_LOAD(XFontsOfFontSet);
	X11_FUNC_LOAD(XBaseFontNameListOfFontSet);
	X11_FUNC_LOAD(XLocaleOfFontSet);
	X11_FUNC_LOAD(XContextDependentDrawing);
	X11_FUNC_LOAD(XDirectionalDependentDrawing);
	X11_FUNC_LOAD(XContextualDrawing);
	X11_FUNC_LOAD(XExtentsOfFontSet);
	X11_FUNC_LOAD(XmbTextEscapement);
	X11_FUNC_LOAD(XwcTextEscapement);
	X11_FUNC_LOAD(Xutf8TextEscapement);
	X11_FUNC_LOAD(XmbTextExtents);
	X11_FUNC_LOAD(XwcTextExtents);
	X11_FUNC_LOAD(Xutf8TextExtents);
	X11_FUNC_LOAD(XmbTextPerCharExtents);
	X11_FUNC_LOAD(XwcTextPerCharExtents);
	X11_FUNC_LOAD(Xutf8TextPerCharExtents);
	X11_FUNC_LOAD(XmbDrawText);
	X11_FUNC_LOAD(XwcDrawText);
	X11_FUNC_LOAD(Xutf8DrawText);
	X11_FUNC_LOAD(XmbDrawString);
	X11_FUNC_LOAD(XwcDrawString);
	X11_FUNC_LOAD(Xutf8DrawString);
	X11_FUNC_LOAD(XmbDrawImageString);
	X11_FUNC_LOAD(XwcDrawImageString);
	X11_FUNC_LOAD(Xutf8DrawImageString);
	X11_FUNC_LOAD(XOpenIM);
	X11_FUNC_LOAD(XCloseIM);
	X11_FUNC_LOAD(XGetIMValues);
	X11_FUNC_LOAD(XSetIMValues);
	X11_FUNC_LOAD(XDisplayOfIM);
	X11_FUNC_LOAD(XLocaleOfIM);
	X11_FUNC_LOAD(XCreateIC);
	X11_FUNC_LOAD(XDestroyIC);
	X11_FUNC_LOAD(XSetICFocus);
	X11_FUNC_LOAD(XUnsetICFocus);
	X11_FUNC_LOAD(XwcResetIC);
	X11_FUNC_LOAD(XmbResetIC);
	X11_FUNC_LOAD(Xutf8ResetIC);
	X11_FUNC_LOAD(XSetICValues);
	X11_FUNC_LOAD(XGetICValues);
	X11_FUNC_LOAD(XIMOfIC);
	X11_FUNC_LOAD(XFilterEvent);
	X11_FUNC_LOAD(XmbLookupString);
	X11_FUNC_LOAD(XwcLookupString);
	X11_FUNC_LOAD(Xutf8LookupString);
	X11_FUNC_LOAD(XVaCreateNestedList);
	X11_FUNC_LOAD(XRegisterIMInstantiateCallback);
	X11_FUNC_LOAD(XUnregisterIMInstantiateCallback);
	X11_FUNC_LOAD(XInternalConnectionNumbers);
	X11_FUNC_LOAD(XProcessInternalConnection);
	X11_FUNC_LOAD(XAddConnectionWatch);
	X11_FUNC_LOAD(XRemoveConnectionWatch);
	X11_FUNC_LOAD(XSetAuthorization);
	X11_FUNC_LOAD(_Xmbtowc);
	X11_FUNC_LOAD(_Xwctomb);
	X11_FUNC_LOAD(XGetEventData);
	X11_FUNC_LOAD(XFreeEventData);

	X11_FUNC_LOAD(XAllocClassHint);
	X11_FUNC_LOAD(XAllocIconSize);
	X11_FUNC_LOAD(XAllocSizeHints);
	X11_FUNC_LOAD(XAllocStandardColormap);
	X11_FUNC_LOAD(XAllocWMHints);
	X11_FUNC_LOAD(XClipBox);
	X11_FUNC_LOAD(XCreateRegion);
	X11_FUNC_LOAD(XDefaultString);
	X11_FUNC_LOAD(XDeleteContext);
	X11_FUNC_LOAD(XDestroyRegion);
	X11_FUNC_LOAD(XEmptyRegion);
	X11_FUNC_LOAD(XEqualRegion);
	X11_FUNC_LOAD(XFindContext);
	X11_FUNC_LOAD(XGetClassHint);
	X11_FUNC_LOAD(XGetIconSizes);
	X11_FUNC_LOAD(XGetNormalHints);
	X11_FUNC_LOAD(XGetRGBColormaps);
	X11_FUNC_LOAD(XGetSizeHints);
	X11_FUNC_LOAD(XGetStandardColormap);
	X11_FUNC_LOAD(XGetTextProperty);
	X11_FUNC_LOAD(XGetVisualInfo);
	X11_FUNC_LOAD(XGetWMClientMachine);
	X11_FUNC_LOAD(XGetWMHints);
	X11_FUNC_LOAD(XGetWMIconName);
	X11_FUNC_LOAD(XGetWMName);
	X11_FUNC_LOAD(XGetWMNormalHints);
	X11_FUNC_LOAD(XGetWMSizeHints);
	X11_FUNC_LOAD(XGetZoomHints);
	X11_FUNC_LOAD(XIntersectRegion);
	X11_FUNC_LOAD(XConvertCase);
	X11_FUNC_LOAD(XLookupString);
	X11_FUNC_LOAD(XMatchVisualInfo);
	X11_FUNC_LOAD(XOffsetRegion);
	X11_FUNC_LOAD(XPointInRegion);
	X11_FUNC_LOAD(XPolygonRegion);
	X11_FUNC_LOAD(XRectInRegion);
	X11_FUNC_LOAD(XSaveContext);
	X11_FUNC_LOAD(XSetClassHint);
	X11_FUNC_LOAD(XSetIconSizes);
	X11_FUNC_LOAD(XSetNormalHints);
	X11_FUNC_LOAD(XSetRGBColormaps);
	X11_FUNC_LOAD(XSetSizeHints);
	X11_FUNC_LOAD(XSetStandardProperties);
	X11_FUNC_LOAD(XSetTextProperty);
	X11_FUNC_LOAD(XSetWMClientMachine);
	X11_FUNC_LOAD(XSetWMHints);
	X11_FUNC_LOAD(XSetWMIconName);
	X11_FUNC_LOAD(XSetWMName);
	X11_FUNC_LOAD(XSetWMNormalHints);
	X11_FUNC_LOAD(XSetWMProperties);
	X11_FUNC_LOAD(XmbSetWMProperties);
	X11_FUNC_LOAD(Xutf8SetWMProperties);
	X11_FUNC_LOAD(XSetWMSizeHints);
	X11_FUNC_LOAD(XSetRegion);
	X11_FUNC_LOAD(XSetStandardColormap);
	X11_FUNC_LOAD(XSetZoomHints);
	X11_FUNC_LOAD(XShrinkRegion);
	X11_FUNC_LOAD(XStringListToTextProperty);
	X11_FUNC_LOAD(XSubtractRegion);
	X11_FUNC_LOAD(XmbTextListToTextProperty);
	X11_FUNC_LOAD(XwcTextListToTextProperty);
	X11_FUNC_LOAD(Xutf8TextListToTextProperty);
	X11_FUNC_LOAD(XwcFreeStringList);
	X11_FUNC_LOAD(XTextPropertyToStringList);
	X11_FUNC_LOAD(XmbTextPropertyToTextList);
	X11_FUNC_LOAD(XwcTextPropertyToTextList);
	X11_FUNC_LOAD(Xutf8TextPropertyToTextList);
	X11_FUNC_LOAD(XUnionRectWithRegion);
	X11_FUNC_LOAD(XUnionRegion);
	X11_FUNC_LOAD(XWMGeometry);
	X11_FUNC_LOAD(XXorRegion);

#if USE_XRENDER
#define XRENDER_FUNC_LOAD(x) x = MwDynamicSymbol(xsymtbl.lib_xrender, #x);

	XRENDER_FUNC_LOAD(XRenderQueryExtension)
	XRENDER_FUNC_LOAD(XRenderQueryVersion)
	XRENDER_FUNC_LOAD(XRenderQueryFormats)
	XRENDER_FUNC_LOAD(XRenderQuerySubpixelOrder)
	XRENDER_FUNC_LOAD(XRenderSetSubpixelOrder)
	XRENDER_FUNC_LOAD(XRenderFindVisualFormat)
	XRENDER_FUNC_LOAD(XRenderFindFormat)
	XRENDER_FUNC_LOAD(XRenderFindStandardFormat)
	XRENDER_FUNC_LOAD(XRenderQueryPictIndexValues)
	XRENDER_FUNC_LOAD(XRenderCreatePicture)
	XRENDER_FUNC_LOAD(XRenderFreePicture)
	XRENDER_FUNC_LOAD(XRenderChangePicture)
	XRENDER_FUNC_LOAD(XRenderSetPictureClipRectangles)
	XRENDER_FUNC_LOAD(XRenderSetPictureClipRegion)
	XRENDER_FUNC_LOAD(XRenderSetPictureTransform)
	XRENDER_FUNC_LOAD(XRenderComposite)
	XRENDER_FUNC_LOAD(XRenderCreateGlyphSet)
	XRENDER_FUNC_LOAD(XRenderReferenceGlyphSet)
	XRENDER_FUNC_LOAD(XRenderFreeGlyphSet)
	XRENDER_FUNC_LOAD(XRenderAddGlyphs)
	XRENDER_FUNC_LOAD(XRenderFreeGlyphs)
	XRENDER_FUNC_LOAD(XRenderCompositeString8)
	XRENDER_FUNC_LOAD(XRenderCompositeString16)
	XRENDER_FUNC_LOAD(XRenderCompositeString32)
	XRENDER_FUNC_LOAD(XRenderCompositeText8)
	XRENDER_FUNC_LOAD(XRenderCompositeText16)
	XRENDER_FUNC_LOAD(XRenderCompositeText32)
	XRENDER_FUNC_LOAD(XRenderFillRectangle)
	XRENDER_FUNC_LOAD(XRenderFillRectangles)
	XRENDER_FUNC_LOAD(XRenderCompositeTrapezoids)
	XRENDER_FUNC_LOAD(XRenderCompositeTriangles)
	XRENDER_FUNC_LOAD(XRenderCompositeTriStrip)
	XRENDER_FUNC_LOAD(XRenderCompositeTriFan)
	XRENDER_FUNC_LOAD(XRenderCompositeDoublePoly)
	XRENDER_FUNC_LOAD(XRenderParseColor)
	XRENDER_FUNC_LOAD(XRenderCreateCursor)
	XRENDER_FUNC_LOAD(XRenderQueryFilters)
	XRENDER_FUNC_LOAD(XRenderSetPictureFilter)
	XRENDER_FUNC_LOAD(XRenderCreateAnimCursor)
	XRENDER_FUNC_LOAD(XRenderAddTraps)
	XRENDER_FUNC_LOAD(XRenderCreateSolidFill)
	XRENDER_FUNC_LOAD(XRenderCreateLinearGradient)
	XRENDER_FUNC_LOAD(XRenderCreateRadialGradient)
	XRENDER_FUNC_LOAD(XRenderCreateConicalGradient)
#endif
	return 0;
}

#include "call.c"
CALL(X11);

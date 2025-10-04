/* $Id$ */

// This is a copy of a bunch of headers from Apple's old SDK with irrelvant things removed.
// The reason this exists is for compiling under modern XCode (which doesn't have these headers).
// As such, our only goal here is that the function pointers can be loaded under PowerPC Mac OS X.
// We don't care about things like compiling for 68k, MetroWorks, etc.
// For Older Mac targets, we will just use the actual headers.
//
// Copyright:  © 1985-2001 by Apple Computer, Inc., all rights reserved

#ifndef __PRE_QUARTZ__
#define __PRE_QUARTZ__

typedef unsigned char  UInt8;
typedef signed char    SInt8;
typedef unsigned short UInt16;
typedef signed short   SInt16;
typedef unsigned long  UInt32;
typedef signed long    SInt32;
#if TARGET_RT_BIG_ENDIAN
struct wide {
	SInt32 hi;
	UInt32 lo;
};
typedef struct wide wide;
struct UnsignedWide {
	UInt32 hi;
	UInt32 lo;
};
typedef struct UnsignedWide UnsignedWide;
#else
struct wide {
	UInt32 lo;
	SInt32 hi;
};
typedef struct wide wide;
struct UnsignedWide {
	UInt32 lo;
	UInt32 hi;
};
typedef struct UnsignedWide UnsignedWide;
#endif
#if TYPE_LONGLONG
typedef signed long long   SInt64;
typedef unsigned long long UInt64;
#else
typedef wide	     SInt64;
typedef UnsignedWide UInt64;
#endif
typedef long	       Fixed;
typedef Fixed*	       FixedPtr;
typedef long	       Fract;
typedef Fract*	       FractPtr;
typedef unsigned long  UnsignedFixed;
typedef UnsignedFixed* UnsignedFixedPtr;
typedef short	       ShortFixed;
typedef ShortFixed*    ShortFixedPtr;
typedef float	       Float32;

typedef struct Float32Point Float32Point;
typedef char*		    Ptr;
typedef Ptr*		    Handle;
typedef long		    Size;
typedef SInt16		    OSErr;
typedef SInt32		    OSStatus;
typedef void*		    LogicalAddress;
typedef const void*	    ConstLogicalAddress;
typedef void*		    PhysicalAddress;
typedef UInt8*		    BytePtr;
typedef UInt32		    ByteCount;
typedef UInt32		    ByteOffset;
typedef SInt32		    Duration;
typedef UnsignedWide	    AbsoluteTime;
typedef UInt32		    OptionBits;
typedef UInt32		    ItemCount;
typedef UInt32		    PBVersion;
typedef SInt16		    ScriptCode;
typedef SInt16		    LangCode;
typedef SInt16		    RegionCode;
typedef unsigned long	    FourCharCode;
typedef FourCharCode	    OSType;
typedef FourCharCode	    ResType;
typedef OSType*		    OSTypePtr;
typedef ResType*	    ResTypePtr;
enum {
	false = 0,
	true  = 1
};
typedef unsigned char Boolean;
typedef long (*PFN_ProcPtr)();
typedef struct RoutineDescriptor* UniversalProcPtr;
typedef UInt32			  UnicodeScalarValue;
typedef UInt32			  UTF32Char;
typedef UInt16			  UniChar;
typedef UInt16			  UTF16Char;
typedef UInt8			  UTF8Char;
typedef UniChar*		  UniCharPtr;
typedef UInt32			  UniCharCount;
typedef UniCharCount*		  UniCharCountPtr;
typedef unsigned char		  Str255[256];
typedef unsigned char		  Str63[64];
typedef unsigned char		  Str32[33];
typedef unsigned char		  Str31[32];
typedef unsigned char		  Str27[28];
typedef unsigned char		  Str15[16];
typedef unsigned char		  Str32Field[34];
typedef Str63			  StrFileName;
typedef unsigned char*		  StringPtr;
typedef StringPtr*		  StringHandle;
typedef const unsigned char*	  ConstStringPtr;
typedef const unsigned char*	  ConstStr255Param;
typedef const unsigned char*	  ConstStr63Param;
typedef const unsigned char*	  ConstStr32Param;
typedef const unsigned char*	  ConstStr31Param;
typedef const unsigned char*	  ConstStr27Param;
typedef const unsigned char*	  ConstStr15Param;
typedef ConstStr63Param		  ConstStrFileNameParam;
struct Point {
	short v;
	short h;
};
typedef struct Point Point;
typedef Point*	     PointPtr;
struct Rect {
	short top;
	short left;
	short bottom;
	short right;
};
typedef struct Rect   Rect;
typedef Rect*	      RectPtr;
typedef unsigned char Style;
typedef short	      StyleParameter;
typedef Style	      StyleField;
typedef UInt8	      Byte;
typedef SInt8	      SignedByte;

enum {
	kAppleManufacturer	    = 'appl',
	kComponentResourceType	    = 'thng',
	kComponentAliasResourceType = 'thga'
};
enum {
	kAnyComponentType	  = 0,
	kAnyComponentSubType	  = 0,
	kAnyComponentManufacturer = 0,
	kAnyComponentFlagsMask	  = 0
};
enum {
	cmpIsMissing		= 1L << 29,
	cmpWantsRegisterMessage = 1L << 31
};
enum {
	kComponentOpenSelect		   = -1,
	kComponentCloseSelect		   = -2,
	kComponentCanDoSelect		   = -3,
	kComponentVersionSelect		   = -4,
	kComponentRegisterSelect	   = -5,
	kComponentTargetSelect		   = -6,
	kComponentUnregisterSelect	   = -7,
	kComponentGetMPWorkFunctionSelect  = -8,
	kComponentExecuteWiredActionSelect = -9,
	kComponentGetPublicResourceSelect  = -10
};
enum {
	componentDoAutoVersion		 = (1 << 0),
	componentWantsUnregister	 = (1 << 1),
	componentAutoVersionIncludeFlags = (1 << 2),
	componentHasMultiplePlatforms	 = (1 << 3),
	componentLoadResident		 = (1 << 4)
};
enum {
	defaultComponentIdentical			  = 0,
	defaultComponentAnyFlags			  = 1,
	defaultComponentAnyManufacturer			  = 2,
	defaultComponentAnySubType			  = 4,
	defaultComponentAnyFlagsAnyManufacturer		  = (defaultComponentAnyFlags + defaultComponentAnyManufacturer),
	defaultComponentAnyFlagsAnyManufacturerAnySubType = (defaultComponentAnyFlags + defaultComponentAnyManufacturer + defaultComponentAnySubType)
};
enum {
	registerComponentGlobal	       = 1,
	registerComponentNoDuplicates  = 2,
	registerComponentAfterExisting = 4,
	registerComponentAliasesOnly   = 8
};
struct ComponentDescription {
	OSType	      componentType;
	OSType	      componentSubType;
	OSType	      componentManufacturer;
	unsigned long componentFlags;
	unsigned long componentFlagsMask;
};
typedef struct ComponentDescription ComponentDescription;
struct ResourceSpec {
	OSType resType;
	short  resID;
};
typedef struct ResourceSpec ResourceSpec;
struct ComponentResource {
	ComponentDescription cd;
	ResourceSpec	     component;
	ResourceSpec	     componentName;
	ResourceSpec	     componentInfo;
	ResourceSpec	     componentIcon;
};
typedef struct ComponentResource ComponentResource;
typedef ComponentResource*	 ComponentResourcePtr;
typedef ComponentResourcePtr*	 ComponentResourceHandle;
struct ComponentPlatformInfo {
	long	     componentFlags;
	ResourceSpec component;
	short	     platformType;
};
typedef struct ComponentPlatformInfo ComponentPlatformInfo;
struct ComponentResourceExtension {
	long  componentVersion;
	long  componentRegisterFlags;
	short componentIconFamily;
};
typedef struct ComponentResourceExtension ComponentResourceExtension;
struct ComponentPlatformInfoArray {
	long		      count;
	ComponentPlatformInfo platformArray[1];
};
typedef struct ComponentPlatformInfoArray ComponentPlatformInfoArray;
struct ExtComponentResource {
	ComponentDescription  cd;
	ResourceSpec	      component;
	ResourceSpec	      componentName;
	ResourceSpec	      componentInfo;
	ResourceSpec	      componentIcon;
	long		      componentVersion;
	long		      componentRegisterFlags;
	short		      componentIconFamily;
	long		      count;
	ComponentPlatformInfo platformArray[1];
};
typedef struct ExtComponentResource ExtComponentResource;
typedef ExtComponentResource*	    ExtComponentResourcePtr;
typedef ExtComponentResourcePtr*    ExtComponentResourceHandle;
struct ComponentAliasResource {
	ComponentResource    cr;
	ComponentDescription aliasCD;
};
typedef struct ComponentAliasResource ComponentAliasResource;
struct ComponentParameters {
	UInt8 flags;
	UInt8 paramSize;
	short what;
	long  params[1];
};
typedef struct ComponentParameters ComponentParameters;
struct ComponentRecord {
	long data[1];
};
typedef struct ComponentRecord ComponentRecord;
typedef ComponentRecord*       Component;
struct ComponentInstanceRecord {
	long data[1];
};
typedef struct ComponentInstanceRecord ComponentInstanceRecord;
typedef ComponentInstanceRecord*       ComponentInstance;
struct RegisteredComponentRecord {
	long data[1];
};
typedef struct RegisteredComponentRecord RegisteredComponentRecord;
typedef RegisteredComponentRecord*	 RegisteredComponentRecordPtr;
struct RegisteredComponentInstanceRecord {
	long data[1];
};
typedef struct RegisteredComponentInstanceRecord   RegisteredComponentInstanceRecord;
typedef RegisteredComponentInstanceRecord*	   RegisteredComponentInstanceRecordPtr;
typedef long					   ComponentResult;
typedef struct ComponentMPWorkFunctionHeaderRecord ComponentMPWorkFunctionHeaderRecord;
typedef ComponentMPWorkFunctionHeaderRecord*	   ComponentMPWorkFunctionHeaderRecordPtr;
typedef ComponentResult (*PFN_ComponentMPWorkFunctionProcPtr)(void* globalRefCon, ComponentMPWorkFunctionHeaderRecordPtr header);
typedef ComponentResult (*PFN_ComponentRoutineProcPtr)(ComponentParameters* cp, Handle componentStorage);
typedef OSErr (*PFN_GetMissingComponentResourceProcPtr)(Component c, OSType resType, short resID, void* refCon, Handle* resource);
typedef UniversalProcPtr ComponentFunctionUPP;
typedef unsigned short	 CallingConventionType;
typedef unsigned short	 registerSelectorType;
typedef unsigned long	 ProcInfoType;
typedef unsigned short	 RoutineFlagsType;
typedef UInt8		 RDFlagsType;

typedef OSStatus (*PFN_StandardGlyphs)(
    void*     dataStream,
    ByteCount size);
enum {
	leftCaret  = 0,
	rightCaret = -1,
	kHilite	   = 1
};
enum {
	smLeftCaret  = 0,
	smRightCaret = -1,
	smHilite     = 1
};
enum {
	onlyStyleRun	 = 0,
	leftStyleRun	 = 1,
	rightStyleRun	 = 2,
	middleStyleRun	 = 3,
	smOnlyStyleRun	 = 0,
	smLeftStyleRun	 = 1,
	smRightStyleRun	 = 2,
	smMiddleStyleRun = 3
};
typedef short JustStyleCode;
typedef short TruncCode;
enum {
	truncEnd      = 0,
	truncMiddle   = 0x4000,
	smTruncEnd    = 0,
	smTruncMiddle = 0x4000
};
enum {
	notTruncated   = 0,
	truncated      = 1,
	truncErr       = -1,
	smNotTruncated = 0,
	smTruncated    = 1,
	smTruncErr     = -1
};
typedef SInt8 StyledLineBreakCode;
enum {
	smBreakWord	= 0,
	smBreakChar	= 1,
	smBreakOverflow = 2
};
enum {
	tfAntiAlias = 1 << 0,
	tfUnicode   = 1 << 1
};
struct FontInfo {
	short ascent;
	short descent;
	short widMax;
	short leading;
};
typedef struct FontInfo FontInfo;
typedef short		FormatOrder[1];
typedef FormatOrder*	FormatOrderPtr;
typedef Boolean (*StyleRunDirectionProcPtr)(short styleRunIndex, void* dirParam);
typedef UniversalProcPtr StyleRunDirectionUPP;
typedef StyleRunDirectionUPP (*NewStyleRunDirectionUPP)(StyleRunDirectionProcPtr userRoutine);
#if !OPAQUE_UPP_TYPES
enum {
	uppStyleRunDirectionProcInfo = 0x00000390
};
#define NewStyleRunDirectionUPP(userRoutine) (StyleRunDirectionUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppStyleRunDirectionProcInfo, GetCurrentArchitecture())
#endif
typedef void (*PFN_DisposeStyleRunDirectionUPP)(StyleRunDirectionUPP userUPP);
#if !OPAQUE_UPP_TYPES
#define DisposeStyleRunDirectionUPP(userUPP) DisposeRoutineDescriptor(userUPP)
#endif
typedef Boolean (*PFN_InvokeStyleRunDirectionUPP)(
    short		 styleRunIndex,
    void*		 dirParam,
    StyleRunDirectionUPP userUPP);
#if !OPAQUE_UPP_TYPES
#define InvokeStyleRunDirectionUPP(styleRunIndex, dirParam, userUPP) (Boolean) CALL_TWO_PARAMETER_UPP((userUPP), uppStyleRunDirectionProcInfo, (styleRunIndex), (dirParam))
#endif

enum {
	invalColReq = -1
};
enum {
	srcCopy		   = 0,
	srcOr		   = 1,
	srcXor		   = 2,
	srcBic		   = 3,
	notSrcCopy	   = 4,
	notSrcOr	   = 5,
	notSrcXor	   = 6,
	notSrcBic	   = 7,
	patCopy		   = 8,
	patOr		   = 9,
	patXor		   = 10,
	patBic		   = 11,
	notPatCopy	   = 12,
	notPatOr	   = 13,
	notPatXor	   = 14,
	notPatBic	   = 15,
	grayishTextOr	   = 49,
	hilitetransfermode = 50,
	hilite		   = 50,
	blend		   = 32,
	addPin		   = 33,
	addOver		   = 34,
	subPin		   = 35,
	addMax		   = 37,
	adMax		   = 37,
	subOver		   = 38,
	adMin		   = 39,
	ditherCopy	   = 64,
	transparent	   = 36
};
enum {
	italicBit   = 1,
	ulineBit    = 2,
	outlineBit  = 3,
	shadowBit   = 4,
	condenseBit = 5,
	extendBit   = 6
};
enum {
	normalBit  = 0,
	inverseBit = 1,
	redBit	   = 4,
	greenBit   = 3,
	blueBit	   = 2,
	cyanBit	   = 8,
	magentaBit = 7,
	yellowBit  = 6,
	blackBit   = 5
};
enum {
	blackColor   = 33,
	whiteColor   = 30,
	redColor     = 205,
	greenColor   = 341,
	blueColor    = 409,
	cyanColor    = 273,
	magentaColor = 137,
	yellowColor  = 69
};
enum {
	picLParen  = 0,
	picRParen  = 1,
	clutType   = 0,
	fixedType  = 1,
	directType = 2,
	gdDevType  = 0
};
enum {
	interlacedDevice = 2,
	roundedDevice	 = 5,
	hasAuxMenuBar	 = 6,
	burstDevice	 = 7,
	ext32Device	 = 8,
	ramInit		 = 10,
	mainScreen	 = 11,
	allInit		 = 12,
	screenDevice	 = 13,
	noDriver	 = 14,
	screenActive	 = 15,
	hiliteBit	 = 7,
	pHiliteBit	 = 0,
	defQDColors	 = 127,
	RGBDirect	 = 16,
	baseAddr32	 = 4
};
enum {
	sysPatListID = 0,
	iBeamCursor  = 1,
	crossCursor  = 2,
	plusCursor   = 3,
	watchCursor  = 4
};
enum {
	kQDGrafVerbFrame  = 0,
	kQDGrafVerbPaint  = 1,
	kQDGrafVerbErase  = 2,
	kQDGrafVerbInvert = 3,
	kQDGrafVerbFill	  = 4
};
typedef SInt8 GrafVerb;
enum {
	chunky	     = 0,
	chunkyPlanar = 1,
	planar	     = 2
};
typedef SInt8 PixelType;
typedef short Bits16[16];
struct Pattern {
	UInt8 pat[8];
};
typedef struct Pattern Pattern;
typedef const Pattern* ConstPatternParam;
typedef Pattern*       PatPtr;
typedef PatPtr*	       PatHandle;
typedef SignedByte     QDByte;
typedef QDByte*	       QDPtr;
typedef QDPtr*	       QDHandle;
typedef short	       QDErr;
enum {
	singleDevicesBit  = 0,
	dontMatchSeedsBit = 1,
	allDevicesBit	  = 2
};
enum {
	singleDevices  = 1 << singleDevicesBit,
	dontMatchSeeds = 1 << dontMatchSeedsBit,
	allDevices     = 1 << allDevicesBit
};
typedef unsigned long DeviceLoopFlags;
typedef SInt32	      PrinterStatusOpcode;
enum {
	kPrinterFontStatus    = 0,
	kPrinterScalingStatus = 1
};
struct PrinterFontStatus {
	SInt32 oResult;
	SInt16 iFondID;
	Style  iStyle;
};
typedef struct PrinterFontStatus PrinterFontStatus;
struct PrinterScalingStatus {
	Point oScalingFactors;
};
typedef struct PrinterScalingStatus PrinterScalingStatus;
struct BitMap {
	Ptr   baseAddr;
	short rowBytes;
	Rect  bounds;
};
typedef struct BitMap BitMap;
typedef BitMap*	      BitMapPtr;
typedef BitMapPtr*    BitMapHandle;
struct MacCursor {
	Bits16 data;
	Bits16 mask;
	Point  hotSpot;
};
typedef struct MacCursor MacCursor;
typedef MacCursor*	 CursPtr;
typedef CursPtr*	 CursHandle;
struct PenState {
	Point	pnLoc;
	Point	pnSize;
	short	pnMode;
	Pattern pnPat;
};
typedef struct PenState PenState;
#if !OPAQUE_TOOLBOX_STRUCTS
struct MacRegion {
	unsigned short rgnSize;
	Rect	       rgnBBox;
};
typedef struct MacRegion MacRegion;
typedef MacRegion*	 RgnPtr;
typedef RgnPtr*		 RgnHandle;
#else
typedef struct OpaqueRgnHandle* RgnHandle;
#endif
struct Picture {
	short picSize;
	Rect  picFrame;
};
typedef struct MacPicture MacPicture;
typedef MacPicture*	  PicPtr;
typedef PicPtr*		  PicHandle;
struct MacPolygon {
	short polySize;
	Rect  polyBBox;
	Point polyPoints[1];
};
typedef struct MacPolygon MacPolygon;
typedef MacPolygon	  Polygon;
typedef MacPolygon*	  PolyPtr;
typedef PolyPtr*	  PolyHandle;
typedef void (*PFN_QDTextProcPtr)(short byteCount, const void* textBuf, Point numer, Point denom);
typedef void (*PFN_QDLineProcPtr)(Point newPt);
typedef void (*PFN_QDRectProcPtr)(GrafVerb verb, const Rect* r);
typedef void (*PFN_QDRRectProcPtr)(GrafVerb verb, const Rect* r, short ovalWidth, short ovalHeight);
typedef void (*PFN_QDOvalProcPtr)(GrafVerb verb, const Rect* r);
typedef void (*PFN_QDArcProcPtr)(GrafVerb verb, const Rect* r, short startAngle, short arcAngle);
typedef void (*PFN_QDPolyProcPtr)(GrafVerb verb, PolyHandle poly);
typedef void (*PFN_QDRgnProcPtr)(GrafVerb verb, RgnHandle rgn);
typedef void (*PFN_QDBitsProcPtr)(const BitMap* srcBits, const Rect* srcRect, const Rect* dstRect, short mode, RgnHandle maskRgn);
typedef void (*PFN_QDCommentProcPtr)(short kind, short dataSize, Handle dataHandle);
typedef short (*PFN_QDTxMeasProcPtr)(short byteCount, const void* textAddr, Point* numer, Point* denom, FontInfo* info);
typedef void (*PFN_QDGetPicProcPtr)(void* dataPtr, short byteCount);
typedef void (*PFN_QDPutPicProcPtr)(const void* dataPtr, short byteCount);
typedef void (*PFN_QDOpcodeProcPtr)(const Rect* fromRect, const Rect* toRect, UInt16 opcode, SInt16 version);
typedef OSStatus (*PFN_QDStdGlyphsProcPtr)(void* dataStream, ByteCount size);
typedef void (*PFN_QDJShieldCursorProcPtr)(short left, short top, short right, short bottom);
typedef UniversalProcPtr QDTextUPP;
typedef UniversalProcPtr QDLineUPP;
typedef UniversalProcPtr QDRectUPP;
typedef UniversalProcPtr QDRRectUPP;
typedef UniversalProcPtr QDOvalUPP;
typedef UniversalProcPtr QDArcUPP;
typedef UniversalProcPtr QDPolyUPP;
typedef UniversalProcPtr QDRgnUPP;
typedef UniversalProcPtr QDBitsUPP;
typedef UniversalProcPtr QDCommentUPP;
typedef UniversalProcPtr QDTxMeasUPP;
typedef UniversalProcPtr QDGetPicUPP;
typedef UniversalProcPtr QDPutPicUPP;
typedef UniversalProcPtr QDOpcodeUPP;
typedef UniversalProcPtr QDStdGlyphsUPP;
typedef UniversalProcPtr QDJShieldCursorUPP;
struct QDProcs {
	QDTextUPP    textProc;
	QDLineUPP    lineProc;
	QDRectUPP    rectProc;
	QDRRectUPP   rRectProc;
	QDOvalUPP    ovalProc;
	QDArcUPP     arcProc;
	QDPolyUPP    polyProc;
	QDRgnUPP     rgnProc;
	QDBitsUPP    bitsProc;
	QDCommentUPP commentProc;
	QDTxMeasUPP  txMeasProc;
	QDGetPicUPP  getPicProc;
	QDPutPicUPP  putPicProc;
};
typedef struct QDProcs	  QDProcs;
typedef QDProcs*	  QDProcsPtr;
typedef struct CGContext* CGContextRef;

struct GrafPort {
	short	   device;
	BitMap	   portBits;
	Rect	   portRect;
	RgnHandle  visRgn;
	RgnHandle  clipRgn;
	Pattern	   bkPat;
	Pattern	   fillPat;
	Point	   pnLoc;
	Point	   pnSize;
	short	   pnMode;
	Pattern	   pnPat;
	short	   pnVis;
	short	   txFont;
	StyleField txFace;
	short	   txMode;
	short	   txSize;
	Fixed	   spExtra;
	long	   fgColor;
	long	   bkColor;
	short	   colrBit;
	short	   patStretch;
	Handle	   picSave;
	Handle	   rgnSave;
	Handle	   polySave;
	QDProcsPtr grafProcs;
};
typedef struct GrafPort GrafPort;
typedef GrafPort*	GrafPtr;
typedef GrafPtr		WindowPtr;
typedef WindowPtr	DialogPtr;
struct RGBColor {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};
typedef struct RGBColor	 RGBColor;
typedef UniversalProcPtr ColorSearchUPP;
typedef UniversalProcPtr ColorComplementUPP;

struct ColorSpec {
	short	 value;
	RGBColor rgb;
};
typedef struct ColorSpec  ColorSpec;
typedef ColorSpec*	  ColorSpecPtr;
typedef ColorSpec	  CSpecArray[1];
typedef struct ColorTable ColorTable;
typedef ColorTable*	  CTabPtr;
typedef CTabPtr*	  CTabHandle;
typedef struct PixMap	  PixMap;
typedef PixMap*		  PixMapPtr;
typedef PixMapPtr*	  PixMapHandle;
struct PixPat {
	short	     patType;
	PixMapHandle patMap;
	Handle	     patData;
	Handle	     patXData;
	short	     patXValid;
	Handle	     patXMap;
	Pattern	     pat1Data;
};
typedef struct PixPat PixPat;
typedef PixPat*	      PixPatPtr;
typedef PixPatPtr*    PixPatHandle;
typedef struct CCrsr  CCrsr;
typedef CCrsr*	      CCrsrPtr;
typedef CCrsrPtr*     CCrsrHandle;
struct ITab {
	long  iTabSeed;
	short iTabRes;
	Byte  iTTable[1];
};
typedef struct ITab	 ITab;
typedef ITab*		 ITabPtr;
typedef ITabPtr*	 ITabHandle;
typedef struct GDevice	 GDevice;
typedef GDevice*	 GDPtr;
typedef GDPtr*		 GDHandle;
typedef struct CGrafPort CGrafPort;
typedef CGrafPort*	 CGrafPtr;
typedef UniversalProcPtr QDPrinterStatusUPP;
struct CQDProcs {
	QDTextUPP	   textProc;
	QDLineUPP	   lineProc;
	QDRectUPP	   rectProc;
	QDRRectUPP	   rRectProc;
	QDOvalUPP	   ovalProc;
	QDArcUPP	   arcProc;
	QDPolyUPP	   polyProc;
	QDRgnUPP	   rgnProc;
	QDBitsUPP	   bitsProc;
	QDCommentUPP	   commentProc;
	QDTxMeasUPP	   txMeasProc;
	QDGetPicUPP	   getPicProc;
	QDPutPicUPP	   putPicProc;
	QDOpcodeUPP	   opcodeProc;
	UniversalProcPtr   newProc1;
	QDStdGlyphsUPP	   glyphsProc;
	QDPrinterStatusUPP printerStatusProc;
	UniversalProcPtr   newProc4;
	UniversalProcPtr   newProc5;
	UniversalProcPtr   newProc6;
};

typedef struct CQDProcs CQDProcs;
typedef CQDProcs*	CQDProcsPtr;
struct ReqListRec {
	short reqLSize;
	short reqLData[1];
};
typedef struct ReqListRec ReqListRec;
struct OpenCPicParams {
	Rect  srcRect;
	Fixed hRes;
	Fixed vRes;
	short version;
	short reserved1;
	long  reserved2;
};
typedef struct OpenCPicParams OpenCPicParams;
typedef UniversalProcPtr      DeviceLoopDrawingUPP;
typedef SInt32		      QDRegionParseDirection;
typedef UniversalProcPtr      RegionToRectsUPP;
enum {
	colorXorXFer = 52,
	noiseXFer    = 53,
	customXFer   = 54
};
enum {
	kXFer1PixelAtATime	 = 0x00000001,
	kXFerConvertPixelToRGB32 = 0x00000002
};
struct CustomXFerRec {
	UInt32 version;
	void*  srcPixels;
	void*  destPixels;
	void*  resultPixels;
	UInt32 refCon;
	UInt32 pixelSize;
	UInt32 pixelCount;
	Point  firstPixelHV;
	Rect   destBounds;
};
typedef struct CustomXFerRec CustomXFerRec;
typedef CustomXFerRec*	     CustomXFerRecPtr;
typedef void (*CustomXFerProcPtr)(CustomXFerRecPtr info);
enum {
	kCursorComponentsVersion = 0x00010001
};
enum {
	kCursorComponentType = 'curs'
};
enum {
	cursorDoesAnimate	       = 1L << 0,
	cursorDoesHardware	       = 1L << 1,
	cursorDoesUnreadableScreenBits = 1L << 2
};
enum {
	kRenderCursorInHardware = 1L << 0,
	kRenderCursorInSoftware = 1L << 1
};
struct CursorInfo {
	long  version;
	long  capabilities;
	long  animateDuration;
	Rect  bounds;
	Point hotspot;
	long  reserved;
};
typedef struct CursorInfo CursorInfo;
enum {
	kCursorComponentInit	      = 0x0001,
	kCursorComponentGetInfo	      = 0x0002,
	kCursorComponentSetOutputMode = 0x0003,
	kCursorComponentSetData	      = 0x0004,
	kCursorComponentReconfigure   = 0x0005,
	kCursorComponentDraw	      = 0x0006,
	kCursorComponentErase	      = 0x0007,
	kCursorComponentMove	      = 0x0008,
	kCursorComponentAnimate	      = 0x0009,
	kCursorComponentLastReserved  = 0x0050
};

typedef OSErr (*PFN_GetPortCustomXFerProc)(
    CGrafPtr	       port,
    CustomXFerProcPtr* proc,
    UInt32*	       flags,
    UInt32*	       refCon);
typedef OSErr (*PFN_SetPortCustomXFerProc)(
    CGrafPtr	      port,
    CustomXFerProcPtr proc,
    UInt32	      flags,
    UInt32	      refCon);
typedef OSErr (*PFN_OpenCursorComponent)(
    Component	       c,
    ComponentInstance* ci);
typedef OSErr (*PFN_CloseCursorComponent)(ComponentInstance ci);
typedef OSErr (*PFN_SetCursorComponent)(ComponentInstance ci);
typedef OSErr (*PFN_CursorComponentChanged)(ComponentInstance ci);
typedef OSErr (*PFN_CursorComponentSetData)(
    ComponentInstance ci,
    long	      data);
typedef Boolean (*PFN_IsValidPort)(CGrafPtr port);
typedef PixMapHandle (*PFN_GetPortPixMap)(CGrafPtr port);
typedef const BitMap* (*PFN_GetPortBitMapForCopyBits)(CGrafPtr port);
typedef Rect* (*PFN_GetPortBounds)(
    CGrafPtr port,
    Rect*    rect);
typedef RGBColor* (*PFN_GetPortForeColor)(
    CGrafPtr  port,
    RGBColor* foreColor);
typedef RGBColor* (*PFN_GetPortBackColor)(
    CGrafPtr  port,
    RGBColor* backColor);
typedef RGBColor* (*PFN_GetPortOpColor)(
    CGrafPtr  port,
    RGBColor* opColor);
typedef RGBColor* (*PFN_GetPortHiliteColor)(
    CGrafPtr  port,
    RGBColor* hiliteColor);
typedef CQDProcsPtr (*PFN_GetPortGrafProcs)(CGrafPtr port);
typedef short (*PFN_GetPortTextFont)(CGrafPtr port);
typedef Style (*PFN_GetPortTextFace)(CGrafPtr port);
typedef short (*PFN_GetPortTextMode)(CGrafPtr port);
typedef short (*PFN_GetPortTextSize)(CGrafPtr port);
typedef short (*PFN_GetPortChExtra)(CGrafPtr port);
typedef short (*PFN_GetPortFracHPenLocation)(CGrafPtr port);
typedef Fixed (*PFN_GetPortSpExtra)(CGrafPtr port);
typedef short (*PFN_GetPortPenVisibility)(CGrafPtr port);
typedef RgnHandle (*PFN_GetPortVisibleRegion)(
    CGrafPtr  port,
    RgnHandle visRgn);
typedef RgnHandle (*PFN_GetPortClipRegion)(
    CGrafPtr  port,
    RgnHandle clipRgn);
typedef PixPatHandle (*PFN_GetPortBackPixPat)(
    CGrafPtr	 port,
    PixPatHandle backPattern);
typedef PixPatHandle (*PFN_GetPortPenPixPat)(
    CGrafPtr	 port,
    PixPatHandle penPattern);
typedef PixPatHandle (*PFN_GetPortFillPixPat)(
    CGrafPtr	 port,
    PixPatHandle fillPattern);
typedef Point* (*PFN_GetPortPenSize)(
    CGrafPtr port,
    Point*   penSize);
typedef SInt32 (*PFN_GetPortPenMode)(CGrafPtr port);
typedef Point* (*PFN_GetPortPenLocation)(
    CGrafPtr port,
    Point*   penLocation);
typedef Boolean (*PFN_IsPortRegionBeingDefined)(CGrafPtr port);
typedef Boolean (*PFN_IsPortPictureBeingDefined)(CGrafPtr port);
typedef Boolean (*PFN_IsPortPolyBeingDefined)(CGrafPtr port);
typedef Boolean (*PFN_IsPortOffscreen)(CGrafPtr port);
typedef Boolean (*PFN_IsPortColor)(CGrafPtr port);
typedef void (*PFN_SetPortBounds)(
    CGrafPtr	port,
    const Rect* rect);
typedef void (*PFN_SetPortOpColor)(
    CGrafPtr	    port,
    const RGBColor* opColor);
typedef void (*PFN_SetPortGrafProcs)(
    CGrafPtr  port,
    CQDProcs* procs);
typedef void (*PFN_SetPortVisibleRegion)(
    CGrafPtr  port,
    RgnHandle visRgn);
typedef void (*PFN_SetPortClipRegion)(
    CGrafPtr  port,
    RgnHandle clipRgn);
typedef void (*PFN_SetPortPenPixPat)(
    CGrafPtr	 port,
    PixPatHandle penPattern);
typedef void (*PFN_SetPortFillPixPat)(
    CGrafPtr	 port,
    PixPatHandle penPattern);
typedef void (*PFN_SetPortBackPixPat)(
    CGrafPtr	 port,
    PixPatHandle backPattern);
typedef void (*PFN_SetPortPenSize)(
    CGrafPtr port,
    Point    penSize);
typedef void (*PFN_SetPortPenMode)(
    CGrafPtr port,
    SInt32   penMode);
typedef void (*PFN_SetPortFracHPenLocation)(
    CGrafPtr port,
    short    pnLocHFrac);
typedef Rect* (*PFN_GetPixBounds)(
    PixMapHandle pixMap,
    Rect*	 bounds);
typedef short (*PFN_GetPixDepth)(PixMapHandle pixMap);
typedef long (*PFN_GetQDGlobalsRandomSeed)(void);
typedef BitMap* (*PFN_GetQDGlobalsScreenBits)(BitMap* screenBits);
typedef MacCursor* (*PFN_GetQDGlobalsArrow)(MacCursor* arrow);
typedef Pattern* (*PFN_GetQDGlobalsDarkGray)(Pattern* dkGray);
typedef Pattern* (*PFN_GetQDGlobalsLightGray)(Pattern* ltGray);
typedef Pattern* (*PFN_GetQDGlobalsGray)(Pattern* gray);
typedef Pattern* (*PFN_GetQDGlobalsBlack)(Pattern* black);
typedef Pattern* (*PFN_GetQDGlobalsWhite)(Pattern* white);
typedef CGrafPtr (*PFN_GetQDGlobalsThePort)(void);
typedef void (*PFN_SetQDGlobalsRandomSeed)(long randomSeed);
typedef void (*PFN_SetQDGlobalsArrow)(const MacCursor* arrow);
typedef Rect* (*PFN_GetRegionBounds)(
    RgnHandle region,
    Rect*     bounds);
typedef Boolean (*PFN_IsRegionRectangular)(RgnHandle region);
typedef CGrafPtr (*PFN_CreateNewPort)(void);
typedef void (*PFN_DisposePort)(CGrafPtr port);
typedef void (*PFN_SetQDError)(OSErr err);
typedef Boolean (*PFN_QDIsPortBuffered)(CGrafPtr port);
typedef Boolean (*PFN_QDIsPortBufferDirty)(CGrafPtr port);
typedef void (*PFN_QDFlushPortBuffer)(
    CGrafPtr  port,
    RgnHandle region);
typedef OSStatus (*PFN_QDGetDirtyRegion)(
    CGrafPtr  port,
    RgnHandle rgn);
typedef OSStatus (*PFN_QDSetDirtyRegion)(
    CGrafPtr  port,
    RgnHandle rgn);
typedef OSStatus (*PFN_CreateCGContextForPort)(
    CGrafPtr	  inPort,
    CGContextRef* outContext);
typedef OSStatus (*PFN_ClipCGContextToRegion)(
    CGContextRef gc,
    const Rect*	 portRect,
    RgnHandle	 region);
typedef OSStatus (*PFN_SyncCGContextOriginWithPort)(
    CGContextRef inContext,
    CGrafPtr	 port);
typedef CGrafPtr (*PFN_CreateNewPortForCGDisplayID)(UInt32 inCGDisplayID);
typedef void (*PFN_QDDisplayWaitCursor)(Boolean forceWaitCursor);
typedef void (*PFN_QDSetPatternOrigin)(Point origin);
typedef void (*PFN_QDGetPatternOrigin)(Point* origin);
typedef SInt16 (*PFN_LMGetScrVRes)(void);
typedef void (*PFN_LMSetScrVRes)(SInt16 value);
typedef SInt16 (*PFN_LMGetScrHRes)(void);
typedef void (*PFN_LMSetScrHRes)(SInt16 value);
typedef GDHandle (*PFN_LMGetMainDevice)(void);
typedef void (*PFN_LMSetMainDevice)(GDHandle value);
typedef GDHandle (*PFN_LMGetDeviceList)(void);
typedef void (*PFN_LMSetDeviceList)(GDHandle value);
typedef Handle (*PFN_LMGetQDColors)(void);
typedef void (*PFN_LMSetQDColors)(Handle value);
typedef Handle (*PFN_LMGetWidthListHand)(void);
typedef void (*PFN_LMSetWidthListHand)(Handle value);
typedef UInt8 (*PFN_LMGetHiliteMode)(void);
typedef void (*PFN_LMSetHiliteMode)(UInt8 value);
typedef Ptr (*PFN_LMGetWidthPtr)(void);
typedef void (*PFN_LMSetWidthPtr)(Ptr value);
typedef Handle (*PFN_LMGetWidthTabHandle)(void);
typedef void (*PFN_LMSetWidthTabHandle)(Handle value);
typedef SInt32 (*PFN_LMGetLastSPExtra)(void);
typedef void (*PFN_LMSetLastSPExtra)(SInt32 value);
typedef Handle (*PFN_LMGetLastFOND)(void);
typedef void (*PFN_LMSetLastFOND)(Handle value);
typedef UInt8 (*PFN_LMGetFractEnable)(void);
typedef void (*PFN_LMSetFractEnable)(UInt8 value);
typedef GDHandle (*PFN_LMGetTheGDevice)(void);
typedef void (*PFN_LMSetTheGDevice)(GDHandle value);
typedef void (*PFN_LMGetHiliteRGB)(RGBColor* hiliteRGBValue);
typedef void (*PFN_LMSetHiliteRGB)(const RGBColor* hiliteRGBValue);
typedef Boolean (*PFN_LMGetCursorNew)(void);
typedef void (*PFN_LMSetCursorNew)(Boolean value);
typedef OSStatus (*PFN_QDRegionToRects)(
    RgnHandle		   rgn,
    QDRegionParseDirection dir,
    RegionToRectsUPP	   proc,
    void*		   userData);
typedef void (*PFN_SetPort)(GrafPtr port);
typedef void (*PFN_GetPort)(GrafPtr* port);
typedef void (*PFN_GrafDevice)(short device);
typedef void (*PFN_SetPortBits)(const BitMap* bm);
typedef void (*PFN_PortSize)(
    short width,
    short height);
typedef void (*PFN_MovePortTo)(
    short leftGlobal,
    short topGlobal);
typedef void (*PFN_SetOrigin)(
    short h,
    short v);
typedef void (*PFN_SetClip)(RgnHandle rgn);
typedef void (*PFN_GetClip)(RgnHandle rgn);
typedef void (*PFN_ClipRect)(const Rect* r);
typedef void (*PFN_BackPat)(const Pattern* pat);
typedef void (*PFN_InitCursor)(void);
typedef void (*PFN_SetCursor)(const MacCursor* crsr);
typedef void (*PFN_HideCursor)(void);
typedef void (*PFN_ShowCursor)(void);
typedef void (*PFN_ObscureCursor)(void);
typedef void (*PFN_HidePen)(void);
typedef void (*PFN_ShowPen)(void);
typedef void (*PFN_GetPen)(Point* pt);
typedef void (*PFN_GetPenState)(PenState* pnState);
typedef void (*PFN_SetPenState)(const PenState* pnState);
typedef void (*PFN_PenSize)(
    short width,
    short height);
typedef void (*PFN_PenMode)(short mode);
typedef void (*PFN_PenPat)(const Pattern* pat);
typedef void (*PFN_PenNormal)(void);
typedef void (*PFN_MoveTo)(
    short h,
    short v);
typedef void (*PFN_Move)(
    short dh,
    short dv);
typedef void (*PFN_LineTo)(
    short h,
    short v);
typedef void (*PFN_Line)(
    short dh,
    short dv);
typedef void (*PFN_ForeColor)(long color);
typedef void (*PFN_BackColor)(long color);
typedef void (*PFN_ColorBit)(short whichBit);
typedef void (*PFN_SetRect)(
    Rect* r,
    short left,
    short top,
    short right,
    short bottom);
typedef void (*PFN_OffsetRect)(
    Rect* r,
    short dh,
    short dv);
typedef void (*PFN_InsetRect)(
    Rect* r,
    short dh,
    short dv);
typedef Boolean (*PFN_SectRect)(
    const Rect* src1,
    const Rect* src2,
    Rect*	dstRect);
typedef void (*PFN_UnionRect)(
    const Rect* src1,
    const Rect* src2,
    Rect*	dstRect);
typedef Boolean (*PFN_EqualRect)(
    const Rect* rect1,
    const Rect* rect2);
typedef Boolean (*PFN_EmptyRect)(const Rect* r);
typedef void (*PFN_FrameRect)(const Rect* r);
typedef void (*PFN_PaintRect)(const Rect* r);
typedef void (*PFN_EraseRect)(const Rect* r);
typedef void (*PFN_InvertRect)(const Rect* r);
typedef void (*PFN_FillRect)(
    const Rect*	   r,
    const Pattern* pat);
typedef void (*PFN_FrameOval)(const Rect* r);
typedef void (*PFN_PaintOval)(const Rect* r);
typedef void (*PFN_EraseOval)(const Rect* r);
typedef void (*PFN_InvertOval)(const Rect* r);
typedef void (*PFN_FillOval)(
    const Rect*	   r,
    const Pattern* pat);
typedef void (*PFN_FrameRoundRect)(
    const Rect* r,
    short	ovalWidth,
    short	ovalHeight);
typedef void (*PFN_PaintRoundRect)(
    const Rect* r,
    short	ovalWidth,
    short	ovalHeight);
typedef void (*PFN_EraseRoundRect)(
    const Rect* r,
    short	ovalWidth,
    short	ovalHeight);
typedef void (*PFN_InvertRoundRect)(
    const Rect* r,
    short	ovalWidth,
    short	ovalHeight);
typedef void (*PFN_FillRoundRect)(
    const Rect*	   r,
    short	   ovalWidth,
    short	   ovalHeight,
    const Pattern* pat);
typedef void (*PFN_FrameArc)(
    const Rect* r,
    short	startAngle,
    short	arcAngle);
typedef void (*PFN_PaintArc)(
    const Rect* r,
    short	startAngle,
    short	arcAngle);
typedef void (*PFN_EraseArc)(
    const Rect* r,
    short	startAngle,
    short	arcAngle);
typedef void (*PFN_InvertArc)(
    const Rect* r,
    short	startAngle,
    short	arcAngle);
typedef void (*PFN_FillArc)(
    const Rect*	   r,
    short	   startAngle,
    short	   arcAngle,
    const Pattern* pat);
typedef RgnHandle (*PFN_NewRgn)(void);
typedef void (*PFN_OpenRgn)(void);
typedef void (*PFN_CloseRgn)(RgnHandle dstRgn);
typedef OSErr (*PFN_BitMapToRegion)(
    RgnHandle	  region,
    const BitMap* bMap);
typedef void (*PFN_HandleToRgn)(
    Handle    oldRegion,
    RgnHandle region);
typedef void (*PFN_RgnToHandle)(
    RgnHandle region,
    Handle    flattenedRgnDataHdl);
typedef void (*PFN_DisposeRgn)(RgnHandle rgn);
typedef void (*PFN_CopyRgn)(
    RgnHandle srcRgn,
    RgnHandle dstRgn);
typedef void (*PFN_SetEmptyRgn)(RgnHandle rgn);
typedef void (*PFN_SetRectRgn)(
    RgnHandle rgn,
    short     left,
    short     top,
    short     right,
    short     bottom);
typedef void (*PFN_RectRgn)(
    RgnHandle	rgn,
    const Rect* r);
typedef void (*PFN_OffsetRgn)(
    RgnHandle rgn,
    short     dh,
    short     dv);
typedef void (*PFN_InsetRgn)(
    RgnHandle rgn,
    short     dh,
    short     dv);
typedef void (*PFN_SectRgn)(
    RgnHandle srcRgnA,
    RgnHandle srcRgnB,
    RgnHandle dstRgn);
typedef void (*PFN_UnionRgn)(
    RgnHandle srcRgnA,
    RgnHandle srcRgnB,
    RgnHandle dstRgn);
typedef void (*PFN_DiffRgn)(
    RgnHandle srcRgnA,
    RgnHandle srcRgnB,
    RgnHandle dstRgn);
typedef void (*PFN_XorRgn)(
    RgnHandle srcRgnA,
    RgnHandle srcRgnB,
    RgnHandle dstRgn);
typedef Boolean (*PFN_RectInRgn)(
    const Rect* r,
    RgnHandle	rgn);
typedef Boolean (*PFN_EqualRgn)(
    RgnHandle rgnA,
    RgnHandle rgnB);
typedef Boolean (*PFN_EmptyRgn)(RgnHandle rgn);
typedef void (*PFN_FrameRgn)(RgnHandle rgn);
typedef void (*PFN_PaintRgn)(RgnHandle rgn);
typedef void (*PFN_EraseRgn)(RgnHandle rgn);
typedef void (*PFN_InvertRgn)(RgnHandle rgn);
typedef void (*PFN_FillRgn)(
    RgnHandle	   rgn,
    const Pattern* pat);
typedef void (*PFN_ScrollRect)(
    const Rect* r,
    short	dh,
    short	dv,
    RgnHandle	updateRgn);
typedef void (*PFN_CopyBits)(
    const BitMap* srcBits,
    const BitMap* dstBits,
    const Rect*	  srcRect,
    const Rect*	  dstRect,
    short	  mode,
    RgnHandle	  maskRgn);
typedef void (*PFN_SeedFill)(
    const void* srcPtr,
    void*	dstPtr,
    short	srcRow,
    short	dstRow,
    short	height,
    short	words,
    short	seedH,
    short	seedV);
typedef void (*PFN_CalcMask)(
    const void* srcPtr,
    void*	dstPtr,
    short	srcRow,
    short	dstRow,
    short	height,
    short	words);
typedef void (*PFN_CopyMask)(
    const BitMap* srcBits,
    const BitMap* maskBits,
    const BitMap* dstBits,
    const Rect*	  srcRect,
    const Rect*	  maskRect,
    const Rect*	  dstRect);
typedef PicHandle (*PFN_OpenPicture)(const Rect* picFrame);
typedef void (*PFN_PicComment)(
    short  kind,
    short  dataSize,
    Handle dataHandle);
typedef void (*PFN_ClosePicture)(void);
typedef void (*PFN_DrawPicture)(
    PicHandle	myPicture,
    const Rect* dstRect);
typedef void (*PFN_KillPicture)(PicHandle myPicture);
typedef PolyHandle (*PFN_OpenPoly)(void);
typedef void (*PFN_ClosePoly)(void);
typedef void (*PFN_KillPoly)(PolyHandle poly);
typedef void (*PFN_OffsetPoly)(
    PolyHandle poly,
    short      dh,
    short      dv);
typedef void (*PFN_FramePoly)(PolyHandle poly);
typedef void (*PFN_PaintPoly)(PolyHandle poly);
typedef void (*PFN_ErasePoly)(PolyHandle poly);
typedef void (*PFN_InvertPoly)(PolyHandle poly);
typedef void (*PFN_FillPoly)(
    PolyHandle	   poly,
    const Pattern* pat);
typedef void (*PFN_SetPt)(
    Point* pt,
    short  h,
    short  v);
typedef void (*PFN_LocalToGlobal)(Point* pt);
typedef void (*PFN_GlobalToLocal)(Point* pt);
typedef short (*PFN_Random)(void);
typedef void (*PFN_StuffHex)(
    void*	     thingPtr,
    ConstStr255Param s);
typedef Boolean (*PFN_GetPixel)(
    short h,
    short v);
typedef void (*PFN_ScalePt)(
    Point*	pt,
    const Rect* srcRect,
    const Rect* dstRect);
typedef void (*PFN_MapPt)(
    Point*	pt,
    const Rect* srcRect,
    const Rect* dstRect);
typedef void (*PFN_MapRect)(
    Rect*	r,
    const Rect* srcRect,
    const Rect* dstRect);
typedef void (*PFN_MapRgn)(
    RgnHandle	rgn,
    const Rect* srcRect,
    const Rect* dstRect);
typedef void (*PFN_MapPoly)(
    PolyHandle	poly,
    const Rect* srcRect,
    const Rect* dstRect);
typedef void (*PFN_SetStdProcs)(QDProcs* procs);
typedef void (*PFN_StdRect)(
    GrafVerb	verb,
    const Rect* r);
typedef void (*PFN_StdRRect)(
    GrafVerb	verb,
    const Rect* r,
    short	ovalWidth,
    short	ovalHeight);
typedef void (*PFN_StdOval)(
    GrafVerb	verb,
    const Rect* r);
typedef void (*PFN_StdArc)(
    GrafVerb	verb,
    const Rect* r,
    short	startAngle,
    short	arcAngle);
typedef void (*PFN_StdPoly)(
    GrafVerb   verb,
    PolyHandle poly);
typedef void (*PFN_StdRgn)(
    GrafVerb  verb,
    RgnHandle rgn);
typedef void (*PFN_StdBits)(
    const BitMap* srcBits,
    const Rect*	  srcRect,
    const Rect*	  dstRect,
    short	  mode,
    RgnHandle	  maskRgn);
typedef void (*PFN_StdComment)(
    short  kind,
    short  dataSize,
    Handle dataHandle);
typedef void (*PFN_StdGetPic)(
    void* dataPtr,
    short byteCount);
typedef void (*PFN_StdPutPic)(
    const void* dataPtr,
    short	byteCount);
typedef void (*PFN_StdOpcode)(
    const Rect* fromRect,
    const Rect* toRect,
    UInt16	opcode,
    SInt16	version);
typedef void (*PFN_AddPt)(
    Point  src,
    Point* dst);
typedef Boolean (*PFN_EqualPt)(
    Point pt1,
    Point pt2);
typedef Boolean (*PFN_PtInRect)(
    Point	pt,
    const Rect* r);
typedef void (*PFN_Pt2Rect)(
    Point pt1,
    Point pt2,
    Rect* dstRect);
typedef void (*PFN_PtToAngle)(
    const Rect* r,
    Point	pt,
    short*	angle);
typedef void (*PFN_SubPt)(
    Point  src,
    Point* dst);
typedef Boolean (*PFN_PtInRgn)(
    Point     pt,
    RgnHandle rgn);
typedef void (*PFN_StdLine)(Point newPt);
typedef PixMapHandle (*PFN_NewPixMap)(void);
typedef void (*PFN_DisposePixMap)(PixMapHandle pm);
typedef void (*PFN_CopyPixMap)(
    PixMapHandle srcPM,
    PixMapHandle dstPM);
typedef PixPatHandle (*PFN_NewPixPat)(void);
typedef void (*PFN_DisposePixPat)(PixPatHandle pp);
typedef void (*PFN_CopyPixPat)(
    PixPatHandle srcPP,
    PixPatHandle dstPP);
typedef void (*PFN_PenPixPat)(PixPatHandle pp);
typedef void (*PFN_BackPixPat)(PixPatHandle pp);
typedef PixPatHandle (*PFN_GetPixPat)(short patID);
typedef void (*PFN_MakeRGBPat)(
    PixPatHandle    pp,
    const RGBColor* myColor);
typedef void (*PFN_FillCRect)(
    const Rect*	 r,
    PixPatHandle pp);
typedef void (*PFN_FillCOval)(
    const Rect*	 r,
    PixPatHandle pp);
typedef void (*PFN_FillCRoundRect)(
    const Rect*	 r,
    short	 ovalWidth,
    short	 ovalHeight,
    PixPatHandle pp);
typedef void (*PFN_FillCArc)(
    const Rect*	 r,
    short	 startAngle,
    short	 arcAngle,
    PixPatHandle pp);
typedef void (*PFN_FillCRgn)(
    RgnHandle	 rgn,
    PixPatHandle pp);
typedef void (*PFN_FillCPoly)(
    PolyHandle	 poly,
    PixPatHandle pp);
typedef void (*PFN_RGBForeColor)(const RGBColor* color);
typedef void (*PFN_RGBBackColor)(const RGBColor* color);
typedef void (*PFN_SetCPixel)(
    short	    h,
    short	    v,
    const RGBColor* cPix);
typedef void (*PFN_SetPortPix)(PixMapHandle pm);
typedef void (*PFN_GetCPixel)(
    short     h,
    short     v,
    RGBColor* cPix);
typedef void (*PFN_GetForeColor)(RGBColor* color);
typedef void (*PFN_GetBackColor)(RGBColor* color);
typedef void (*PFN_SeedCFill)(
    const BitMap*  srcBits,
    const BitMap*  dstBits,
    const Rect*	   srcRect,
    const Rect*	   dstRect,
    short	   seedH,
    short	   seedV,
    ColorSearchUPP matchProc,
    long	   matchData);
typedef void (*PFN_CalcCMask)(
    const BitMap*   srcBits,
    const BitMap*   dstBits,
    const Rect*	    srcRect,
    const Rect*	    dstRect,
    const RGBColor* seedRGB,
    ColorSearchUPP  matchProc,
    long	    matchData);
typedef PicHandle (*PFN_OpenCPicture)(const OpenCPicParams* newHeader);
typedef void (*PFN_OpColor)(const RGBColor* color);
typedef void (*PFN_HiliteColor)(const RGBColor* color);
typedef void (*PFN_DisposeCTable)(CTabHandle cTable);
typedef CTabHandle (*PFN_GetCTable)(short ctID);
typedef CCrsrHandle (*PFN_GetCCursor)(short crsrID);
typedef void (*PFN_SetCCursor)(CCrsrHandle cCrsr);
typedef void (*PFN_AllocCursor)(void);
typedef void (*PFN_DisposeCCursor)(CCrsrHandle cCrsr);
typedef void (*PFN_SetStdCProcs)(CQDProcs* procs);
typedef GDHandle (*PFN_GetMaxDevice)(const Rect* globalRect);
typedef long (*PFN_GetCTSeed)(void);
typedef GDHandle (*PFN_GetDeviceList)(void);
typedef GDHandle (*PFN_GetMainDevice)(void);
typedef GDHandle (*PFN_GetNextDevice)(GDHandle curDevice);
typedef Boolean (*PFN_TestDeviceAttribute)(
    GDHandle gdh,
    short    attribute);
typedef void (*PFN_SetDeviceAttribute)(
    GDHandle gdh,
    short    attribute,
    Boolean  value);
typedef void (*PFN_InitGDevice)(
    short    qdRefNum,
    long     mode,
    GDHandle gdh);
typedef GDHandle (*PFN_NewGDevice)(
    short refNum,
    long  mode);
typedef void (*PFN_DisposeGDevice)(GDHandle gdh);
typedef void (*PFN_SetGDevice)(GDHandle gd);
typedef GDHandle (*PFN_GetGDevice)(void);
typedef long (*PFN_Color2Index)(const RGBColor* myColor);
typedef void (*PFN_Index2Color)(
    long      index,
    RGBColor* aColor);
typedef void (*PFN_InvertColor)(RGBColor* myColor);
typedef Boolean (*PFN_RealColor)(const RGBColor* color);
typedef void (*PFN_GetSubTable)(
    CTabHandle myColors,
    short      iTabRes,
    CTabHandle targetTbl);
typedef void (*PFN_MakeITable)(
    CTabHandle cTabH,
    ITabHandle iTabH,
    short      res);
typedef void (*PFN_AddSearch)(ColorSearchUPP searchProc);
typedef void (*PFN_AddComp)(ColorComplementUPP compProc);
typedef void (*PFN_DelSearch)(ColorSearchUPP searchProc);
typedef void (*PFN_DelComp)(ColorComplementUPP compProc);
typedef void (*PFN_SetClientID)(short id);
typedef void (*PFN_ProtectEntry)(
    short   index,
    Boolean protect);
typedef void (*PFN_ReserveEntry)(
    short   index,
    Boolean reserve);
typedef void (*PFN_SetEntries)(
    short      start,
    short      count,
    CSpecArray aTable);
typedef void (*PFN_SaveEntries)(
    CTabHandle	srcTable,
    CTabHandle	resultTable,
    ReqListRec* selection);
typedef void (*PFN_RestoreEntries)(
    CTabHandle	srcTable,
    CTabHandle	dstTable,
    ReqListRec* selection);
typedef short (*PFN_QDError)(void);
typedef void (*PFN_CopyDeepMask)(
    const BitMap* srcBits,
    const BitMap* maskBits,
    const BitMap* dstBits,
    const Rect*	  srcRect,
    const Rect*	  maskRect,
    const Rect*	  dstRect,
    short	  mode,
    RgnHandle	  maskRgn);
typedef void (*PFN_DeviceLoop)(
    RgnHandle		 drawingRgn,
    DeviceLoopDrawingUPP drawingProc,
    long		 userData,
    DeviceLoopFlags	 flags);
typedef Ptr (*PFN_GetMaskTable)(void);
typedef PatHandle (*PFN_GetPattern)(short patternID);
typedef CursHandle (*PFN_GetCursor)(short cursorID);
typedef PicHandle (*PFN_GetPicture)(short pictureID);
typedef long (*PFN_DeltaPoint)(
    Point ptA,
    Point ptB);
typedef void (*PFN_ShieldCursor)(
    const Rect* shieldRect,
    Point	offsetPt);
typedef void (*PFN_ScreenRes)(
    short* scrnHRes,
    short* scrnVRes);
typedef void (*PFN_GetIndPattern)(
    Pattern* thePat,
    short    patternListID,
    short    index);
typedef long (*PFN_deltapoint)(
    Point* ptA,
    Point* ptB);
typedef void (*PFN_PackBits)(
    Ptr*  srcPtr,
    Ptr*  dstPtr,
    short srcBytes);
typedef void (*PFN_UnpackBits)(
    Ptr*  srcPtr,
    Ptr*  dstPtr,
    short dstBytes);
typedef Fixed (*PFN_SlopeFromAngle)(short angle);
typedef short (*PFN_AngleFromSlope)(Fixed slope);

#endif

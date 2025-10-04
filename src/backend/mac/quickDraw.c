/* $Id$ */

#include "mac.h"
#include "quickDraw.h"
#include <dlfcn.h>

void quickDrawBackendUserDataInit(mac_backend_userdata ud) {
	void* carbonLib = dlopen("CarbonLib", RTLD_LAZY | RTLD_LOCAL);

	ud = malloc(sizeof(struct mac_backend_userdata_t));

#define LOAD_QD_FUNC(name) ud->name = dlsym(carbonLib, #name)

	LOAD_QD_FUNC(GetPortCustomXFerProc);
	LOAD_QD_FUNC(SetPortCustomXFerProc);
	LOAD_QD_FUNC(OpenCursorComponent);
	LOAD_QD_FUNC(CloseCursorComponent);
	LOAD_QD_FUNC(SetCursorComponent);
	LOAD_QD_FUNC(CursorComponentChanged);
	LOAD_QD_FUNC(CursorComponentSetData);
	LOAD_QD_FUNC(IsValidPort);
	LOAD_QD_FUNC(GetPortPixMap);
	LOAD_QD_FUNC(GetPortBitMapForCopyBits);
	LOAD_QD_FUNC(GetPortBounds);
	LOAD_QD_FUNC(GetPortForeColor);
	LOAD_QD_FUNC(GetPortBackColor);
	LOAD_QD_FUNC(GetPortOpColor);
	LOAD_QD_FUNC(GetPortHiliteColor);
	LOAD_QD_FUNC(GetPortGrafProcs);
	LOAD_QD_FUNC(GetPortTextFont);
	LOAD_QD_FUNC(GetPortTextFace);
	LOAD_QD_FUNC(GetPortTextMode);
	LOAD_QD_FUNC(GetPortTextSize);
	LOAD_QD_FUNC(GetPortChExtra);
	LOAD_QD_FUNC(GetPortFracHPenLocation);
	LOAD_QD_FUNC(GetPortSpExtra);
	LOAD_QD_FUNC(GetPortPenVisibility);
	LOAD_QD_FUNC(GetPortVisibleRegion);
	LOAD_QD_FUNC(GetPortClipRegion);
	LOAD_QD_FUNC(GetPortBackPixPat);
	LOAD_QD_FUNC(GetPortPenPixPat);
	LOAD_QD_FUNC(GetPortFillPixPat);
	LOAD_QD_FUNC(GetPortPenSize);
	LOAD_QD_FUNC(GetPortPenMode);
	LOAD_QD_FUNC(GetPortPenLocation);
	LOAD_QD_FUNC(IsPortRegionBeingDefined);
	LOAD_QD_FUNC(IsPortPictureBeingDefined);
	LOAD_QD_FUNC(IsPortPolyBeingDefined);
	LOAD_QD_FUNC(IsPortOffscreen);
	LOAD_QD_FUNC(IsPortColor);
	LOAD_QD_FUNC(SetPortBounds);
	LOAD_QD_FUNC(SetPortOpColor);
	LOAD_QD_FUNC(SetPortGrafProcs);
	LOAD_QD_FUNC(SetPortVisibleRegion);
	LOAD_QD_FUNC(SetPortClipRegion);
	LOAD_QD_FUNC(SetPortPenPixPat);
	LOAD_QD_FUNC(SetPortFillPixPat);
	LOAD_QD_FUNC(SetPortBackPixPat);
	LOAD_QD_FUNC(SetPortPenSize);
	LOAD_QD_FUNC(SetPortPenMode);
	LOAD_QD_FUNC(SetPortFracHPenLocation);
	LOAD_QD_FUNC(GetPixBounds);
	LOAD_QD_FUNC(GetPixDepth);
	LOAD_QD_FUNC(GetQDGlobalsRandomSeed);
	LOAD_QD_FUNC(GetQDGlobalsScreenBits);
	LOAD_QD_FUNC(GetQDGlobalsArrow);
	LOAD_QD_FUNC(GetQDGlobalsDarkGray);
	LOAD_QD_FUNC(GetQDGlobalsLightGray);
	LOAD_QD_FUNC(GetQDGlobalsGray);
	LOAD_QD_FUNC(GetQDGlobalsBlack);
	LOAD_QD_FUNC(GetQDGlobalsWhite);
	LOAD_QD_FUNC(GetQDGlobalsThePort);
	LOAD_QD_FUNC(SetQDGlobalsRandomSeed);
	LOAD_QD_FUNC(SetQDGlobalsArrow);
	LOAD_QD_FUNC(GetRegionBounds);
	LOAD_QD_FUNC(IsRegionRectangular);
	LOAD_QD_FUNC(CreateNewPort);
	LOAD_QD_FUNC(DisposePort);
	LOAD_QD_FUNC(SetQDError);
	LOAD_QD_FUNC(QDIsPortBuffered);
	LOAD_QD_FUNC(QDIsPortBufferDirty);
	LOAD_QD_FUNC(QDFlushPortBuffer);
	LOAD_QD_FUNC(QDGetDirtyRegion);
	LOAD_QD_FUNC(QDSetDirtyRegion);
	LOAD_QD_FUNC(CreateCGContextForPort);
	LOAD_QD_FUNC(ClipCGContextToRegion);
	LOAD_QD_FUNC(SyncCGContextOriginWithPort);
	LOAD_QD_FUNC(CreateNewPortForCGDisplayID);
	LOAD_QD_FUNC(QDDisplayWaitCursor);
	LOAD_QD_FUNC(QDSetPatternOrigin);
	LOAD_QD_FUNC(QDGetPatternOrigin);
	LOAD_QD_FUNC(LMGetScrVRes);
	LOAD_QD_FUNC(LMSetScrVRes);
	LOAD_QD_FUNC(LMGetScrHRes);
	LOAD_QD_FUNC(LMSetScrHRes);
	LOAD_QD_FUNC(LMGetMainDevice);
	LOAD_QD_FUNC(LMSetMainDevice);
	LOAD_QD_FUNC(LMGetDeviceList);
	LOAD_QD_FUNC(LMSetDeviceList);
	LOAD_QD_FUNC(LMGetQDColors);
	LOAD_QD_FUNC(LMSetQDColors);
	LOAD_QD_FUNC(LMGetWidthListHand);
	LOAD_QD_FUNC(LMSetWidthListHand);
	LOAD_QD_FUNC(LMGetHiliteMode);
	LOAD_QD_FUNC(LMSetHiliteMode);
	LOAD_QD_FUNC(LMGetWidthPtr);
	LOAD_QD_FUNC(LMSetWidthPtr);
	LOAD_QD_FUNC(LMGetWidthTabHandle);
	LOAD_QD_FUNC(LMSetWidthTabHandle);
	LOAD_QD_FUNC(LMGetLastSPExtra);
	LOAD_QD_FUNC(LMSetLastSPExtra);
	LOAD_QD_FUNC(LMGetLastFOND);
	LOAD_QD_FUNC(LMSetLastFOND);
	LOAD_QD_FUNC(LMGetFractEnable);
	LOAD_QD_FUNC(LMSetFractEnable);
	LOAD_QD_FUNC(LMGetTheGDevice);
	LOAD_QD_FUNC(LMSetTheGDevice);
	LOAD_QD_FUNC(LMGetHiliteRGB);
	LOAD_QD_FUNC(LMSetHiliteRGB);
	LOAD_QD_FUNC(LMGetCursorNew);
	LOAD_QD_FUNC(LMSetCursorNew);
	LOAD_QD_FUNC(QDRegionToRects);
	LOAD_QD_FUNC(SetPort);
	LOAD_QD_FUNC(GetPort);
	LOAD_QD_FUNC(GrafDevice);
	LOAD_QD_FUNC(SetPortBits);
	LOAD_QD_FUNC(PortSize);
	LOAD_QD_FUNC(MovePortTo);
	LOAD_QD_FUNC(SetOrigin);
	LOAD_QD_FUNC(SetClip);
	LOAD_QD_FUNC(GetClip);
	LOAD_QD_FUNC(ClipRect);
	LOAD_QD_FUNC(BackPat);
	LOAD_QD_FUNC(InitCursor);
	LOAD_QD_FUNC(SetCursor);
	LOAD_QD_FUNC(HideCursor);
	LOAD_QD_FUNC(ShowCursor);
	LOAD_QD_FUNC(ObscureCursor);
	LOAD_QD_FUNC(HidePen);
	LOAD_QD_FUNC(ShowPen);
	LOAD_QD_FUNC(GetPen);
	LOAD_QD_FUNC(GetPenState);
	LOAD_QD_FUNC(SetPenState);
	LOAD_QD_FUNC(PenSize);
	LOAD_QD_FUNC(PenMode);
	LOAD_QD_FUNC(PenPat);
	LOAD_QD_FUNC(PenNormal);
	LOAD_QD_FUNC(MoveTo);
	LOAD_QD_FUNC(Move);
	LOAD_QD_FUNC(LineTo);
	LOAD_QD_FUNC(Line);
	LOAD_QD_FUNC(ForeColor);
	LOAD_QD_FUNC(BackColor);
	LOAD_QD_FUNC(ColorBit);
	LOAD_QD_FUNC(SetRect);
	LOAD_QD_FUNC(OffsetRect);
	LOAD_QD_FUNC(InsetRect);
	LOAD_QD_FUNC(SectRect);
	LOAD_QD_FUNC(UnionRect);
	LOAD_QD_FUNC(EqualRect);
	LOAD_QD_FUNC(EmptyRect);
	LOAD_QD_FUNC(FrameRect);
	LOAD_QD_FUNC(PaintRect);
	LOAD_QD_FUNC(EraseRect);
	LOAD_QD_FUNC(InvertRect);
	LOAD_QD_FUNC(FillRect);
	LOAD_QD_FUNC(FrameOval);
	LOAD_QD_FUNC(PaintOval);
	LOAD_QD_FUNC(EraseOval);
	LOAD_QD_FUNC(InvertOval);
	LOAD_QD_FUNC(FillOval);
	LOAD_QD_FUNC(FrameRoundRect);
	LOAD_QD_FUNC(PaintRoundRect);
	LOAD_QD_FUNC(EraseRoundRect);
	LOAD_QD_FUNC(InvertRoundRect);
	LOAD_QD_FUNC(FillRoundRect);
	LOAD_QD_FUNC(FrameArc);
	LOAD_QD_FUNC(PaintArc);
	LOAD_QD_FUNC(EraseArc);
	LOAD_QD_FUNC(InvertArc);
	LOAD_QD_FUNC(FillArc);
	LOAD_QD_FUNC(NewRgn);
	LOAD_QD_FUNC(OpenRgn);
	LOAD_QD_FUNC(CloseRgn);
	LOAD_QD_FUNC(BitMapToRegion);
	LOAD_QD_FUNC(HandleToRgn);
	LOAD_QD_FUNC(RgnToHandle);
	LOAD_QD_FUNC(DisposeRgn);
	LOAD_QD_FUNC(CopyRgn);
	LOAD_QD_FUNC(SetEmptyRgn);
	LOAD_QD_FUNC(SetRectRgn);
	LOAD_QD_FUNC(RectRgn);
	LOAD_QD_FUNC(OffsetRgn);
	LOAD_QD_FUNC(InsetRgn);
	LOAD_QD_FUNC(SectRgn);
	LOAD_QD_FUNC(UnionRgn);
	LOAD_QD_FUNC(DiffRgn);
	LOAD_QD_FUNC(XorRgn);
	LOAD_QD_FUNC(RectInRgn);
	LOAD_QD_FUNC(EqualRgn);
	LOAD_QD_FUNC(EmptyRgn);
	LOAD_QD_FUNC(FrameRgn);
	LOAD_QD_FUNC(PaintRgn);
	LOAD_QD_FUNC(EraseRgn);
	LOAD_QD_FUNC(InvertRgn);
	LOAD_QD_FUNC(FillRgn);
	LOAD_QD_FUNC(ScrollRect);
	LOAD_QD_FUNC(CopyBits);
	LOAD_QD_FUNC(SeedFill);
	LOAD_QD_FUNC(CalcMask);
	LOAD_QD_FUNC(CopyMask);
	LOAD_QD_FUNC(OpenPicture);
	LOAD_QD_FUNC(PicComment);
	LOAD_QD_FUNC(ClosePicture);
	LOAD_QD_FUNC(DrawPicture);
	LOAD_QD_FUNC(KillPicture);
	LOAD_QD_FUNC(OpenPoly);
	LOAD_QD_FUNC(ClosePoly);
	LOAD_QD_FUNC(KillPoly);
	LOAD_QD_FUNC(OffsetPoly);
	LOAD_QD_FUNC(FramePoly);
	LOAD_QD_FUNC(PaintPoly);
	LOAD_QD_FUNC(ErasePoly);
	LOAD_QD_FUNC(InvertPoly);
	LOAD_QD_FUNC(FillPoly);
	LOAD_QD_FUNC(SetPt);
	LOAD_QD_FUNC(LocalToGlobal);
	LOAD_QD_FUNC(GlobalToLocal);
	LOAD_QD_FUNC(Random);
	LOAD_QD_FUNC(StuffHex);
	LOAD_QD_FUNC(GetPixel);
	LOAD_QD_FUNC(ScalePt);
	LOAD_QD_FUNC(MapPt);
	LOAD_QD_FUNC(MapRect);
	LOAD_QD_FUNC(MapRgn);
	LOAD_QD_FUNC(MapPoly);
	LOAD_QD_FUNC(SetStdProcs);
	LOAD_QD_FUNC(StdRect);
	LOAD_QD_FUNC(StdRRect);
	LOAD_QD_FUNC(StdOval);
	LOAD_QD_FUNC(StdArc);
	LOAD_QD_FUNC(StdPoly);
	LOAD_QD_FUNC(StdRgn);
	LOAD_QD_FUNC(StdBits);
	LOAD_QD_FUNC(StdComment);
	LOAD_QD_FUNC(StdGetPic);
	LOAD_QD_FUNC(StdPutPic);
	LOAD_QD_FUNC(StdOpcode);
	LOAD_QD_FUNC(AddPt);
	LOAD_QD_FUNC(EqualPt);
	LOAD_QD_FUNC(PtInRect);
	LOAD_QD_FUNC(Pt2Rect);
	LOAD_QD_FUNC(PtToAngle);
	LOAD_QD_FUNC(SubPt);
	LOAD_QD_FUNC(PtInRgn);
	LOAD_QD_FUNC(StdLine);
	LOAD_QD_FUNC(NewPixMap);
	LOAD_QD_FUNC(DisposePixMap);
	LOAD_QD_FUNC(CopyPixMap);
	LOAD_QD_FUNC(NewPixPat);
	LOAD_QD_FUNC(DisposePixPat);
	LOAD_QD_FUNC(CopyPixPat);
	LOAD_QD_FUNC(PenPixPat);
	LOAD_QD_FUNC(BackPixPat);
	LOAD_QD_FUNC(GetPixPat);
	LOAD_QD_FUNC(MakeRGBPat);
	LOAD_QD_FUNC(FillCRect);
	LOAD_QD_FUNC(FillCOval);
	LOAD_QD_FUNC(FillCRoundRect);
	LOAD_QD_FUNC(FillCArc);
	LOAD_QD_FUNC(FillCRgn);
	LOAD_QD_FUNC(FillCPoly);
	LOAD_QD_FUNC(RGBForeColor);
	LOAD_QD_FUNC(RGBBackColor);
	LOAD_QD_FUNC(SetCPixel);
	LOAD_QD_FUNC(SetPortPix);
	LOAD_QD_FUNC(GetCPixel);
	LOAD_QD_FUNC(GetForeColor);
	LOAD_QD_FUNC(GetBackColor);
	LOAD_QD_FUNC(SeedCFill);
	LOAD_QD_FUNC(CalcCMask);
	LOAD_QD_FUNC(OpenCPicture);
	LOAD_QD_FUNC(OpColor);
	LOAD_QD_FUNC(HiliteColor);
	LOAD_QD_FUNC(DisposeCTable);
	LOAD_QD_FUNC(GetCTable);
	LOAD_QD_FUNC(GetCCursor);
	LOAD_QD_FUNC(SetCCursor);
	LOAD_QD_FUNC(AllocCursor);
	LOAD_QD_FUNC(DisposeCCursor);
	LOAD_QD_FUNC(SetStdCProcs);
	LOAD_QD_FUNC(GetMaxDevice);
	LOAD_QD_FUNC(GetCTSeed);
	LOAD_QD_FUNC(GetDeviceList);
	LOAD_QD_FUNC(GetMainDevice);
	LOAD_QD_FUNC(GetNextDevice);
	LOAD_QD_FUNC(TestDeviceAttribute);
	LOAD_QD_FUNC(SetDeviceAttribute);
	LOAD_QD_FUNC(InitGDevice);
	LOAD_QD_FUNC(NewGDevice);
	LOAD_QD_FUNC(DisposeGDevice);
	LOAD_QD_FUNC(SetGDevice);
	LOAD_QD_FUNC(GetGDevice);
	LOAD_QD_FUNC(Color2Index);
	LOAD_QD_FUNC(Index2Color);
	LOAD_QD_FUNC(InvertColor);
	LOAD_QD_FUNC(RealColor);
	LOAD_QD_FUNC(GetSubTable);
	LOAD_QD_FUNC(MakeITable);
	LOAD_QD_FUNC(AddSearch);
	LOAD_QD_FUNC(AddComp);
	LOAD_QD_FUNC(DelSearch);
	LOAD_QD_FUNC(DelComp);
	LOAD_QD_FUNC(SetClientID);
	LOAD_QD_FUNC(ProtectEntry);
	LOAD_QD_FUNC(ReserveEntry);
	LOAD_QD_FUNC(SetEntries);
	LOAD_QD_FUNC(SaveEntries);
	LOAD_QD_FUNC(RestoreEntries);
	LOAD_QD_FUNC(QDError);
	LOAD_QD_FUNC(CopyDeepMask);
	LOAD_QD_FUNC(DeviceLoop);
	LOAD_QD_FUNC(GetMaskTable);
	LOAD_QD_FUNC(GetPattern);
	LOAD_QD_FUNC(GetCursor);
	LOAD_QD_FUNC(GetPicture);
	LOAD_QD_FUNC(DeltaPoint);
	LOAD_QD_FUNC(ShieldCursor);
	LOAD_QD_FUNC(ScreenRes);
	LOAD_QD_FUNC(GetIndPattern);
	LOAD_QD_FUNC(deltapoint);
	LOAD_QD_FUNC(PackBits);
	LOAD_QD_FUNC(UnpackBits);
	LOAD_QD_FUNC(SlopeFromAngle);
	LOAD_QD_FUNC(AngleFromSlope);

#undef LOAD_QD_FUNC
}

static MwLL quickdraw_create(MwLL parent, int x, int y, int width, int height) {
	return NULL;
};
static void quickdraw_destroy(MwLL handle) {
	return;
};
static void quickdraw_polygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	return;
};
static MwLLColor quickdraw_allocColor(MwLL handle, int r, int g, int b) {
	return NULL;
};
static void quickdraw_freeColor(MwLLColor color) {
	return;
};
static void quickdraw_getXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	return;
};
static void quickdraw_setXY(MwLL handle, int x, int y) {
	return;
};
static void quickdraw_setWH(MwLL handle, int w, int h) {
	return;
};
static void quickdraw_setTitle(MwLL handle, const char* title) {
	return;
};
static int quickdraw_pending(MwLL handle) {
	return 0;
};
static void quickdraw_nextEvent(MwLL handle) {
	return;
};
static MwLLPixmap quickdraw_createPixmap(MwLL handle, unsigned char* data, int width, int height) {
	return NULL;
};
static void quickdraw_drawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	return;
};
static void quickdraw_setIcon(MwLL handle, MwLLPixmap pixmap) {
	return;
};
static void quickdraw_forceRender(MwLL handle) {
	return;
};

static mac_backend quickdraw_backend = {
    .create	  = quickdraw_create,
    .destroy	  = quickdraw_destroy,
    .polygon	  = quickdraw_polygon,
    .allocColor	  = quickdraw_allocColor,
    .freeColor	  = quickdraw_freeColor,
    .getXYWH	  = quickdraw_getXYWH,
    .setXY	  = quickdraw_setXY,
    .setWH	  = quickdraw_setWH,
    .setTitle	  = quickdraw_setTitle,
    .pending	  = quickdraw_pending,
    .nextEvent	  = quickdraw_nextEvent,
    .createPixmap = quickdraw_createPixmap,
    .drawPixmap	  = quickdraw_drawPixmap,
    .setIcon	  = quickdraw_setIcon,
    .forceRender  = quickdraw_forceRender,
};

mac_backend getQuickDrawBackend(void) {
	return quickdraw_backend;
};

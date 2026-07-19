/*
 * DirectWrite Font backend
 *
 * This should dynamically load everything necessary and then gracefully fallback if the user isn't on Windows 10.
 */
#ifdef USE_DIRECTWRITE
#define INITGUID
#include <Mw/Milsko.h>

#include <d2d1.h>
#include <shobjidl.h>

#ifndef WINBOOL
#define WINBOOL int
#endif

/* dwrite is vendored because it's only mingw that gives us COBJMACRO */
#include "dwrite.h"
#include "dwrite_3.h"

typedef struct CustomTextRenderer {
	IDWriteTextRendererVtbl*   lpVtbl;
	LONG			   refCount;
	IDWriteFactory6*	   write_factory;
	IDWriteBitmapRenderTarget* target;
	int			   red, blue, green;

} CustomTextRenderer;
static CustomTextRenderer* CustomTextRenderer_Create(IDWriteFactory6* write_factory, IDWriteBitmapRenderTarget* target, MwLLColor color);

static struct dw {
	HMODULE d2d1_lib;
	HMODULE dwrite_lib;

	HRESULT (*D2D1CreateFactory)(D2D1_FACTORY_TYPE factoryType, const IID* const riid, const D2D1_FACTORY_OPTIONS* pFactoryOptions, void** ppIFactory);
	HRESULT (*DWriteCreateFactory)(int factoryType, const IID* const iid, IUnknown** factory);

	HMODULE kernel32_lib;
} dw_table;

struct _MwFLFont {
	int    px;
	void*  data;
	MwBool valid;

	IDWriteFactory6*	write_factory;
	ID2D1Factory*		d2dFactory;
	IDWriteGdiInterop*	gdiInterop;
	IDWriteFontFile*	file;
	IDWriteFontFace*	font_face;
	IDWriteFontSetBuilder2* font_builder;
	IDWriteFontSet*		font_set;
	IDWriteFontCollection1* font_collection;
	IDWriteTextFormat*	text_format;

	LOGFONTW logfont;
	HFONT	 font;
};

static int unpack_ttf_data(WCHAR* temp_path_name, unsigned char* data, unsigned int size) {
	WCHAR  temp_path_dir[MAX_PATH];
	HANDLE tempFile;
	int    hr;

	hr = GetTempPathW(MAX_PATH, temp_path_dir);
	if(hr == 0) {
		printf("GetTempPathW failed for TTF, cannot use DirectWrite\n");
		return 1;
	}
	hr = GetTempFileNameW(temp_path_dir, TEXT(L"MILSKO"), 0, temp_path_name);
	if(hr == 0) {
		printf("GetTempFileName failed for TTF, cannot use DirectWrite\n");
		return 1;
	}

	tempFile = CreateFileW(temp_path_name,
			       GENERIC_WRITE,
			       0,
			       NULL,
			       CREATE_NEW,
			       FILE_ATTRIBUTE_NORMAL,
			       NULL);
	if(tempFile == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		if(err == ERROR_FILE_EXISTS) {
			tempFile = CreateFileW(temp_path_name,
					       GENERIC_WRITE,
					       0,
					       NULL,
					       OPEN_ALWAYS,
					       FILE_ATTRIBUTE_NORMAL,
					       NULL);
			if(tempFile != INVALID_HANDLE_VALUE) {
				goto success;
			} else {
				err = GetLastError();
			}
		}
		printf("CreateFile failed for TTF (%ld), cannot use DirectWrite\n", err);
		return 1;
	}
success:

	hr = WriteFile(tempFile, data, size, NULL, NULL);
	if(!hr) {
		printf("WriteFile failed for TTF (%ld), cannot use DirectWrite\n", GetLastError());
		return 1;
	}

	CloseHandle(tempFile);

	return 0;
}

static void* dw_MwFontLoad(unsigned char* data, unsigned int size, int px) {
	MwFLFont	     ttf = malloc(sizeof(*ttf));
	HRESULT		     hr;
	WCHAR		     font_name[MAX_PATH];
	D2D1_FACTORY_OPTIONS d2d1_options;

	d2d1_options.debugLevel = D2D1_DEBUG_LEVEL_ERROR;

#define INTERFACE_CHECK(x, factory) \
	do { \
		x* rsrc = NULL; \
		if(!SUCCEEDED((hr = x##_QueryInterface(factory, &IID_##x, (void**)&rsrc)))) { \
			printf("Query for interface " #x " failed (%08lX). Cannot use DirectWrite.\n", hr); \
			return NULL; \
		}; \
	} while(0);

	memset(ttf, 0, sizeof(*ttf));
	ttf->data = malloc(size);
	memcpy(ttf->data, data, size);

	ttf->px = px;

	hr = dw_table.D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &IID_ID2D1Factory, &d2d1_options, (void**)&ttf->d2dFactory);
	if(!SUCCEEDED(hr)) {
		printf("D2D1CreateFactory failed for TTF (%08lX), cannot use DirectWrite\n", hr);
		goto fail;
	}

	INTERFACE_CHECK(ID2D1Factory, ttf->d2dFactory);

	hr = dw_table.DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, &IID_IDWriteFactory, (struct IUnknown**)&ttf->write_factory);
	if(!SUCCEEDED(hr)) {
		printf("DWriteCreateFactory failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFactory6, ttf->write_factory);

	if(unpack_ttf_data(font_name, data, size) != 0) {
		goto fail;
	};

	hr = IDWriteFactory6_CreateFontFileReference(ttf->write_factory, font_name, NULL, &ttf->file);
	if(!SUCCEEDED(hr)) {
		printf("CreateFontFileReference failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFontFile, ttf->file);

	hr = IDWriteFactory_CreateFontFace(ttf->write_factory, DWRITE_FONT_FACE_TYPE_TRUETYPE, 1, &ttf->file, 0, DWRITE_FONT_SIMULATIONS_NONE, &ttf->font_face);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFactory_CreateFontFace failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFontFace, ttf->font_face);

	hr = IDWriteFactory6_CreateFontSetBuilder(ttf->write_factory, &ttf->font_builder);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFactory6_CreateFontSetBuilder failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFontSetBuilder2, ttf->font_builder);

	hr = IDWriteFontSetBuilder2_AddFontFile(ttf->font_builder, font_name);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFontSetBuilder2_AddFontFile failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}

	hr = IDWriteFontSetBuilder_CreateFontSet(ttf->font_builder, &ttf->font_set);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFontSetBuilder_CreateFontSet failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFontSet, ttf->font_set);

	hr = IDWriteFactory3_CreateFontCollectionFromFontSet(ttf->write_factory, ttf->font_set, &ttf->font_collection);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFactory3_CreateFontCollectionFromFontSet failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteFontCollection1, ttf->font_collection);

	hr = IDWriteFactory6_GetGdiInterop(ttf->write_factory, &ttf->gdiInterop);
	if(!SUCCEEDED(hr)) {
		printf("GetGdiInterop failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}
	INTERFACE_CHECK(IDWriteGdiInterop, ttf->gdiInterop);

	hr = IDWriteFactory_CreateTextFormat(ttf->write_factory, L"milsko font", (IDWriteFontCollection*)ttf->font_collection, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, px, L"en-US", &ttf->text_format);
	if(!SUCCEEDED(hr)) {
		printf("IDWriteFactory_CreateTextFormat failed for TTF (%08lx), cannot use DirectWrite\n", hr);
		goto fail;
	}

	ttf->valid = MwTRUE;
	return ttf;

fail:
	ttf->valid = MwFALSE;
	return NULL;
}

static int dw_MwTextOffset(MwFLFont ttf, const char* text);

static int dw_MwDrawText(MwWidget handle, MwFLFont ttf, MwPoint* point, const char* text, MwLLColor color) {
	IDWriteTextLayout*	   text_layout;
	IDWriteBitmapRenderTarget* target = NULL;
	HRESULT			   hr;
	RECT			   rc	     = {0};
	HDC			   memoryHDC = NULL;
	DWRITE_FONT_METRICS	   metrics   = {0};
	LONG			   width = 0, height = 0, offset = 0;
	WCHAR*			   wtext = NULL;
	CustomTextRenderer*	   texRenderer;
	size_t			   wtext_len = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);

	wtext = malloc(wtext_len * 2); /* we get a weird buffer overflow when we try to free this later? allocate well above wtext_len then to prevent that. */
	memset(wtext, 0, wtext_len * 2);
	MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, wtext_len);

	IDWriteFontFace_GetMetrics(ttf->font_face, &metrics);

	GetClientRect(handle->lowlevel->gdi.hWnd, &rc);
	MapWindowPoints(handle->lowlevel->gdi.hWnd, GetParent(handle->lowlevel->gdi.hWnd), (LPPOINT)&rc, 2);

	width  = MwTextWidth(handle, ttf, text);
	height = MwTextHeight(handle, ttf, text);
	offset = dw_MwTextOffset(ttf, text);

	hr = IDWriteFactory_CreateTextLayout(ttf->write_factory, wtext, wtext_len, ttf->text_format, width, height, &text_layout);

	IDWriteGdiInterop_CreateBitmapRenderTarget(ttf->gdiInterop, handle->lowlevel->gdi.hDC, width, height, &target);

	texRenderer = CustomTextRenderer_Create(ttf->write_factory, target, color);

	memoryHDC = IDWriteBitmapRenderTarget_GetMemoryDC(target);

	SetBkMode(handle->lowlevel->gdi.hDC, TRANSPARENT);
	BitBlt(memoryHDC, 0, 0, width, height, handle->lowlevel->gdi.hDC, point->x, point->y - (height / 2), SRCCOPY);

	if(SUCCEEDED(hr)) {
		IDWriteTextLayout_Draw(text_layout, target, (IDWriteTextRenderer*)texRenderer, offset, 0);
	}

	BitBlt(handle->lowlevel->gdi.hDC, point->x, point->y - (height / 2), width, height, memoryHDC, 0, 0, SRCCOPY | NOMIRRORBITMAP);

	IDWriteBitmapRenderTarget_Release(target);

	free(wtext);

	return 0;
}

static int dw_MwTextOffset(MwFLFont ttf, const char* text) {
	IDWriteTextLayout*    text_layout;
	DWRITE_FONT_METRICS   metrics	   = {0};
	WCHAR*		      wtext	   = NULL;
	size_t		      wtext_len	   = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
	UINT32*		      codepoints   = malloc(wtext_len * sizeof(UINT32));
	UINT16*		      glyphIndices = malloc(wtext_len * sizeof(UINT16));
	int		      i;
	DWRITE_GLYPH_METRICS* gmetrics;
	int		      tw = 0;

	wtext = malloc(wtext_len * 2); /* we get a weird buffer overflow when we try to free this later? allocate well above wtext_len then to prevent that. */
	memset(wtext, 0, wtext_len * 2);
	MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, wtext_len);

	for(i = 0; i < wtext_len; i++) codepoints[i] = wtext[i];

	IDWriteFontFace_GetMetrics(ttf->font_face, &metrics);

	IDWriteFactory_CreateTextLayout(ttf->write_factory, wtext, wtext_len, ttf->text_format, 0, 0, &text_layout);

	IDWriteFontFace_GetGlyphIndices(ttf->font_face, codepoints, wtext_len, glyphIndices);

	gmetrics = malloc(wtext_len * sizeof(DWRITE_GLYPH_METRICS));

	IDWriteFontFace_GetDesignGlyphMetrics(ttf->font_face, glyphIndices, wtext_len, gmetrics, FALSE);

	for(i = 0; i < strlen(text); i++) {
		int add = (gmetrics[i].leftSideBearing) * ((float)ttf->px / (float)metrics.designUnitsPerEm);
		tw += add;
	}

	free(wtext);

	return tw;
}

static int dw_MwTextWidth(MwFLFont ttf, const char* text) {
	IDWriteTextLayout*    text_layout;
	DWRITE_FONT_METRICS   metrics	   = {0};
	WCHAR*		      wtext	   = NULL;
	size_t		      wtext_len	   = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
	UINT32*		      codepoints   = malloc(wtext_len * sizeof(UINT32));
	UINT16*		      glyphIndices = malloc(wtext_len * sizeof(UINT16));
	int		      i;
	DWRITE_GLYPH_METRICS* gmetrics;
	int		      tw = 0;

	wtext = malloc(wtext_len * 2); /* we get a weird buffer overflow when we try to free this later? allocate well above wtext_len then to prevent that. */
	memset(wtext, 0, wtext_len * 2);
	MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, wtext_len);

	for(i = 0; i < wtext_len; i++) codepoints[i] = wtext[i];

	IDWriteFontFace_GetMetrics(ttf->font_face, &metrics);

	IDWriteFactory_CreateTextLayout(ttf->write_factory, wtext, wtext_len, ttf->text_format, 0, 0, &text_layout);

	IDWriteFontFace_GetGlyphIndices(ttf->font_face, codepoints, wtext_len, glyphIndices);

	gmetrics = malloc(wtext_len * sizeof(DWRITE_GLYPH_METRICS));

	IDWriteFontFace_GetDesignGlyphMetrics(ttf->font_face, glyphIndices, wtext_len, gmetrics, FALSE);

	for(i = 0; i < strlen(text); i++) {
		int add = (gmetrics[i].advanceWidth + gmetrics[i].leftSideBearing) * ((float)ttf->px / (float)metrics.designUnitsPerEm);
		tw += add;
	}

	free(wtext);

	return tw;
}

static int dw_MwTextHeight(MwFLFont ttf, int count) {
	(void)count;
	return (IDWriteTextFormat2_GetFontSize(ttf->text_format) / 72.0f) * GetDeviceCaps(GetDC(NULL), LOGPIXELSY);
}

static void dw_MwFontFree(void* handle) {
	MwFLFont ttf = handle;

	free(ttf->data);
	free(ttf);
}

/* ---- IUnknown ---- */

static HRESULT STDMETHODCALLTYPE CTR_QueryInterface(
    IDWriteTextRenderer* iface, REFIID riid, void** ppvObject) {
	if(!ppvObject)
		return E_POINTER;

	if(IsEqualGUID(riid, &IID_IUnknown) ||
	   IsEqualGUID(riid, &IID_IDWritePixelSnapping) ||
	   IsEqualGUID(riid, &IID_IDWriteTextRenderer)) {
		*ppvObject = iface;
		iface->lpVtbl->AddRef(iface);
		return S_OK;
	}

	*ppvObject = NULL;
	return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE CTR_AddRef(IDWriteTextRenderer* iface) {
	CustomTextRenderer* This = (CustomTextRenderer*)iface;
	return InterlockedIncrement(&This->refCount);
}

static ULONG STDMETHODCALLTYPE CTR_Release(IDWriteTextRenderer* iface) {
	CustomTextRenderer* This = (CustomTextRenderer*)iface;
	LONG		    ref	 = InterlockedDecrement(&This->refCount);
	if(ref == 0)
		HeapFree(GetProcessHeap(), 0, This);
	return (ULONG)ref;
}

/* ---- IDWritePixelSnapping (base interface, treated as "core") ---- */

static HRESULT STDMETHODCALLTYPE CTR_IsPixelSnappingDisabled(
    IDWriteTextRenderer* iface, void* clientDrawingContext, BOOL* isDisabled) {
	(void)iface;
	(void)clientDrawingContext;
	if(!isDisabled)
		return E_POINTER;
	*isDisabled = FALSE;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CTR_GetCurrentTransform(
    IDWriteTextRenderer* iface, void* clientDrawingContext, DWRITE_MATRIX* transform) {
	(void)iface;
	(void)clientDrawingContext;
	if(!transform)
		return E_POINTER;

	transform->m11 = 1.0f;
	transform->m12 = 0.0f;
	transform->m21 = 0.0f;
	transform->m22 = 1.0f;
	transform->dx  = 0.0f;
	transform->dy  = 0.0f;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CTR_GetPixelsPerDip(
    IDWriteTextRenderer* iface, void* clientDrawingContext, FLOAT* pixelsPerDip) {
	(void)iface;
	(void)clientDrawingContext;
	if(!pixelsPerDip)
		return E_POINTER;
	*pixelsPerDip = 1.0f;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE CTR_DrawGlyphRun(
    IDWriteTextRenderer*		iface,
    void*				clientDrawingContext,
    FLOAT				baselineOriginX,
    FLOAT				baselineOriginY,
    DWRITE_MEASURING_MODE		measuringMode,
    DWRITE_GLYPH_RUN const*		glyphRun,
    DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
    IUnknown*				clientDrawingEffect) {
	IDWriteRenderingParams* params;
	CustomTextRenderer*	This = (CustomTextRenderer*)iface;

	(void)clientDrawingContext;
	(void)glyphRunDescription;
	(void)clientDrawingEffect;

	IDWriteFactory6_CreateRenderingParams(This->write_factory, &params);

	return IDWriteBitmapRenderTarget_DrawGlyphRun(This->target, baselineOriginX, baselineOriginY, measuringMode, glyphRun, params, RGB(This->red, This->green, This->blue), NULL);
}

static HRESULT STDMETHODCALLTYPE CTR_DrawUnderline(
    IDWriteTextRenderer*    iface,
    void*		    clientDrawingContext,
    FLOAT		    baselineOriginX,
    FLOAT		    baselineOriginY,
    DWRITE_UNDERLINE const* underline,
    IUnknown*		    clientDrawingEffect) {
	(void)iface;
	(void)clientDrawingContext;
	(void)baselineOriginX;
	(void)baselineOriginY;
	(void)underline;
	(void)clientDrawingEffect;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE CTR_DrawStrikethrough(
    IDWriteTextRenderer*	iface,
    void*			clientDrawingContext,
    FLOAT			baselineOriginX,
    FLOAT			baselineOriginY,
    DWRITE_STRIKETHROUGH const* strikethrough,
    IUnknown*			clientDrawingEffect) {
	(void)iface;
	(void)clientDrawingContext;
	(void)baselineOriginX;
	(void)baselineOriginY;
	(void)strikethrough;
	(void)clientDrawingEffect;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE CTR_DrawInlineObject(
    IDWriteTextRenderer* iface,
    void*		 clientDrawingContext,
    FLOAT		 originX,
    FLOAT		 originY,
    IDWriteInlineObject* inlineObject,
    BOOL		 isSideways,
    BOOL		 isRightToLeft,
    IUnknown*		 clientDrawingEffect) {
	(void)iface;
	(void)clientDrawingContext;
	(void)originX;
	(void)originY;
	(void)inlineObject;
	(void)isSideways;
	(void)isRightToLeft;
	(void)clientDrawingEffect;
	return E_NOTIMPL;
}

/* ---- Vtable instance ---- */

static IDWriteTextRendererVtbl CTR_Vtbl = {
    CTR_QueryInterface,
    CTR_AddRef,
    CTR_Release,
    CTR_IsPixelSnappingDisabled,
    CTR_GetCurrentTransform,
    CTR_GetPixelsPerDip,
    CTR_DrawGlyphRun,
    CTR_DrawUnderline,
    CTR_DrawStrikethrough,
    CTR_DrawInlineObject};

/* ---- Constructor ---- */

static CustomTextRenderer* CustomTextRenderer_Create(IDWriteFactory6* write_factory, IDWriteBitmapRenderTarget* target, MwLLColor color) {
	CustomTextRenderer* obj =
	    (CustomTextRenderer*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					   sizeof(CustomTextRenderer));
	if(!obj)
		return NULL;

	obj->lpVtbl	   = &CTR_Vtbl;
	obj->refCount	   = 1;
	obj->write_factory = write_factory;
	obj->target	   = target;
	obj->red	   = color->common.red;
	obj->green	   = color->common.green;
	obj->blue	   = color->common.blue;

	return obj;
}

int MwFL_DWSetup(void) {
	HRESULT hr;
	HANDLE	ole32lib;
	/* CoInitialize, to my knowledge, is avaliable on every Windows version we actually support. But as of writing, CI fails because a specific mingw won't link to it by default. I'm not gonna waste my time chancing that it will properly link to ole32 either. */
	HRESULT (*CoInitialize)(LPVOID pvReserved);

	ole32lib = LoadLibrary("Ole32.dll");
	if(!ole32lib) {
		printf("ole32lib not found, cannot use DirectWrite\n");
		return 1;
	}
	CoInitialize = (HRESULT(*)(LPVOID pvReserved))GetProcAddress(ole32lib, "CoInitialize");
	if(!CoInitialize) {
		printf("CoInitialize not found, cannot use DirectWrite\n");
		return 1;
	}

	CoInitialize(NULL);

	/* Try and load DirectWrite. If it fails, return 1, signifying we default to bitmap drawing.  */
	dw_table.d2d1_lib = LoadLibrary("D2d1.dll");
	if(!dw_table.d2d1_lib) {
		printf("D2d1.dll not found, cannot use DirectWrite\n");
		return 1;
	}
	dw_table.dwrite_lib = LoadLibrary("dwrite.dll");
	if(!dw_table.dwrite_lib) {
		printf("dwrite.dll not found, cannot use DirectWrite\n");
		return 1;
	}

#define D2D1_LOAD(x) \
	dw_table.x = (typeof(dw_table.x))GetProcAddress(dw_table.d2d1_lib, #x); \
	if(!dw_table.x) { \
		printf("Can't use DirectWrite, " #x " not found."); \
		return 1; \
	}

#define DWRITE_LOAD(x) \
	dw_table.x = (typeof(dw_table.x))GetProcAddress(dw_table.dwrite_lib, #x); \
	if(!dw_table.x) { \
		printf("Can't use DirectWrite, " #x " not found."); \
		return 1; \
	}

	D2D1_LOAD(D2D1CreateFactory);
	DWRITE_LOAD(DWriteCreateFactory);

	MwFLDrawText   = dw_MwDrawText;
	MwFLTextWidth  = dw_MwTextWidth;
	MwFLTextHeight = dw_MwTextHeight;
	MwFLFontLoad   = dw_MwFontLoad;
	MwFLFontFree   = dw_MwFontFree;
	return 0;
}
#endif

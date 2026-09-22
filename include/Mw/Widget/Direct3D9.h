/*!
 * @file Mw/Widget/Direct3D9.h
 * @brief Direct3D9 widget.
 * @warning Only avaliable on Windows.
 */
#ifndef __MW_WIDGET_DIRECT3D9_H__
#define __MW_WIDGET_DIRECT3D9_H__

#include <Mw/Core.h>
#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#if !defined(MW_DIRECT3D9_NO_INCLUDE)

/* https://github.com/microsoft/Windows-classic-samples/issues/317 */
#if defined(__MINGW32__) || defined(__MINGW64__)
#undef NTDDI_VERSION
#undef _WIN32_WINNT

#define NTDDI_VERSION 0x07000000
#define _WIN32_WINNT 0x0A00
#endif

#ifndef _WIN32
#error D3D9 widget only avaliable for Win32 backends
#undef MW_DIRECT3D9
#else
#ifdef __WATCOMC__
#include <windows.h>
#endif
#include <d3d9.h>
#endif
#endif

/*!
 * @brief D3D9 widget class
 */
MWDECL MwClass MwDirect3D9Class;

#ifdef __cplusplus
extern "C" {
#endif

MwInline LPDIRECT3D9 MwDirect3D9GetDirect3D9(MwWidget handle) {
	LPDIRECT3D9 out = NULL;
	MwVaWidgetExecute(handle, "mwDirect3D9GetDirect3D9", &out, NULL);
	return out;
};
MwInline LPDIRECT3DDEVICE9 MwDirect3D9GetDirect3DDevice9(MwWidget handle) {
	LPDIRECT3DDEVICE9 out = NULL;
	MwVaWidgetExecute(handle, "mwDirect3D9GetDirect3DDevice9", &out);
	return out;
};

#ifdef __cplusplus
}
#endif

#endif

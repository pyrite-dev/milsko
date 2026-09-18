/*!
 * @file Mw/Widget/DirectX.h
 * @brief DirectX widget.
 * @warning Only avaliable on Windows.
 */
#ifndef __MW_WIDGET_DIRECTX_H__
#define __MW_WIDGET_DIRECTX_H__

#include <Mw/Core.h>
#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#if !defined(MW_DIRECTX_NO_INCLUDE)

/* https://github.com/microsoft/Windows-classic-samples/issues/317 */
#if defined(__MINGW32__) || defined(__MINGW64__)
#define NTDDI_VERSION 0x07000000
#define _WIN32_WINNT 0x0A00

#endif

#include <d3d9.h>
#endif

#ifndef _WIN32
#error DirectX widget only avaliable for Win32 backends
#endif

/*!
 * @brief D3D9 widget class
 */
MWDECL MwClass MwD3D9Class;

#ifdef __cplusplus
extern "C" {
#endif

MwInline LPDIRECT3D9 MwDirectXGetD3D9(MwWidget handle) {
	LPDIRECT3D9 out = NULL;
	MwVaWidgetExecute(handle, "mwDirectXGetD3D9", &out, NULL);
	return out;
};
MwInline LPDIRECT3DDEVICE9 MwDirectXGetD3Dev9(MwWidget handle) {
	LPDIRECT3DDEVICE9 out = NULL;
	MwVaWidgetExecute(handle, "mwDirectXGetD3Dev9", &out);
	return out;
};

#ifdef __cplusplus
}
#endif

#endif

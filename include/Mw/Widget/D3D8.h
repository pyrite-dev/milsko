/*!
 * @file Mw/Widget/D3D8.h
 * @brief D3D8 widget.
 * @warning Only avaliable on Windows.
 */
#ifndef __MW_WIDGET_D3D8_H__
#define __MW_WIDGET_D3D8_H__

#include <Mw/Core.h>
#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>

#if !defined(MW_D3D8_NO_INCLUDE)

/* https://github.com/microsoft/Windows-classic-samples/issues/317 */
#if defined(__MINGW32__) || defined(__MINGW64__)
#define NTDDI_VERSION 0x07000000
#define _WIN32_WINNT 0x0A00

#endif
#endif

#ifndef _WIN32
#error D3D8 widget only avaliable for Win32 backends
#else
#ifdef MW_DIRECTX8
#include <d3d8.h>
#endif
#endif

/*!
 * @brief D3D8 widget class
 */
MWDECL MwClass MwD3D8Class;

#ifdef __cplusplus
extern "C" {
#endif

MwInline LPDIRECT3D8 MwDirectXGetD3D8(MwWidget handle) {
	LPDIRECT3D8 out = NULL;
	MwVaWidgetExecute(handle, "mwDirectXGetD3D8", &out, NULL);
	return out;
};
MwInline LPDIRECT3DDEVICE8 MwDirectXGetD3Dev8(MwWidget handle) {
	LPDIRECT3DDEVICE8 out = NULL;
	MwVaWidgetExecute(handle, "mwDirectXGetD3Dev8", &out);
	return out;
};

#ifdef __cplusplus
}
#endif

#endif

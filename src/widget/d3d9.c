#include <Mw/Milsko.h>

#ifndef MW_DIRECTX9
#define MW_DIRECTX9_NO_INCLUDE
MWDECL MwClass MwDX9Class;

MwClass MwDX9Class = NULL;
#else
#include <Mw/Widget/D3D9.h>

typedef struct gdid3d9 {
	void* d3d9dll;
	IDirect3D9* (*Direct3DCreate9)(UINT sdk_version);

	LPDIRECT3D9	  d3d;	  // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class\

} gdid3d9_t;

static int wcreate_d3d9(MwWidget handle) {
	void*	   r = NULL;
	MwWidget   w = handle;
	gdid3d9_t* o = malloc(sizeof(gdid3d9_t));

	o->d3d9dll = LoadLibrary("d3d9.dll");
	if(!o->d3d9dll) {
		MessageBox(NULL, "d3d9.dll not found! Is DirectX9 installed?", "Error", MB_OK);
		ExitProcess(-1);
	}
	o->Direct3DCreate9 = (void*)GetProcAddress(o->d3d9dll, "Direct3DCreate9");

	o->d3d = o->Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));		  // clear out the struct for use
	d3dpp.Windowed	    = TRUE;			  // program windowed, not fullscreen
	d3dpp.SwapEffect    = D3DSWAPEFFECT_DISCARD;	  // discard old frames
	d3dpp.hDeviceWindow = handle->lowlevel->gdi.hWnd; // set the window to be used by Direct3D

	// create a device class using this information and information from the d3dpp stuct
	o->d3d->lpVtbl->CreateDevice(o->d3d,
				     D3DADAPTER_DEFAULT,
				     D3DDEVTYPE_HAL,
				     handle->lowlevel->gdi.hWnd,
				     D3DCREATE_MIXED_VERTEXPROCESSING,
				     &d3dpp,
				     &o->d3ddev);

	handle->internal = o;

	return 0;
}

static void destroy_d3d9(MwWidget handle) {
	MwWidget   w = handle;
	gdid3d9_t* o = handle->internal;

	o->d3ddev->lpVtbl->Release(o->d3ddev); // close and release the 3D device
	o->d3d->lpVtbl->Release(o->d3d);       // close and release Direct3D

	free(handle->internal);
}

static void func_handler_d3d9(MwWidget handle, const char* name, void* out,
			      va_list va) {
	gdid3d9_t* o = handle->internal;

	if(strcmp(name, "mwDirectXGetD3D9") == 0) {
		*(LPDIRECT3D9*)out = o->d3d;
	}
	if(strcmp(name, "mwDirectXGetD3Dev9") == 0) {
		*(LPDIRECT3DDEVICE9*)out = o->d3ddev;
	}
}

MwClassRec MwDX9ClassRec = {wcreate_d3d9,      /* create */
			    destroy_d3d9,      /* destroy */
			    NULL,	       /* draw */
			    NULL,	       /* click */
			    NULL,	       /* parent_resize */
			    NULL,	       /* prop_change */
			    NULL,	       /* mouse_move */
			    NULL,	       /* mouse_up */
			    NULL,	       /* mouse_down */
			    NULL,	       /* key */
			    func_handler_d3d9, /* execute */
			    NULL,	       /* tick */
			    NULL,	       /* resize */
			    NULL,	       /* children_update */
			    NULL,	       /* children_prop_change */
			    NULL,	       /* clipboard */
			    NULL,	       /* props_change */
			    NULL, NULL, NULL};
MwClass	   MwD3D9Class	 = &MwDX9ClassRec;
#endif

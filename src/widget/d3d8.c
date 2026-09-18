#include <Mw/Milsko.h>

#ifdef MW_DIRECTX8
#include <Mw/Widget/D3D8.h>
typedef struct gdid3d8 {
	void* d3d8dll;
	IDirect3D8* (*Direct3DCreate8)(UINT sdk_version);

	LPDIRECT3D8	  d3d;	  // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE8 d3ddev; // the pointer to the device class\

} gdid3d8_t;

static int wcreate_d3d8(MwWidget handle) {
	void*		      r = NULL;
	MwWidget	      w = handle;
	gdid3d8_t*	      o = malloc(sizeof(gdid3d8_t));
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE	      dispMode;

	o->d3d8dll = LoadLibrary("d3d8.dll");
	if(!o->d3d8dll) {
		MessageBox(NULL, "d3d8.dll not found! Is DirectX8 installed?", "Error", MB_OK);
		ExitProcess(-1);
	}
	o->Direct3DCreate8 = (void*)GetProcAddress(o->d3d8dll, "Direct3DCreate8");
	if(!o->Direct3DCreate8) {
		MessageBox(NULL, "Direct3DCreate8 not found! Is DirectX8 installed properly?", "Error", MB_OK);
		ExitProcess(-1);
	}

	o->d3d = o->Direct3DCreate8(D3D_SDK_VERSION);

	o->d3d->lpVtbl->GetAdapterDisplayMode(o->d3d, D3DADAPTER_DEFAULT, &dispMode);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed		     = TRUE;
	d3dpp.SwapEffect	     = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow	     = handle->lowlevel->gdi.hWnd;
	d3dpp.BackBufferFormat	     = dispMode.Format;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and information from the d3dpp stuct
	o->d3d->lpVtbl->CreateDevice(o->d3d,
				     D3DADAPTER_DEFAULT,
				     D3DDEVTYPE_HAL,
				     handle->lowlevel->gdi.hWnd,
				     D3DCREATE_MIXED_VERTEXPROCESSING,
				     &d3dpp,
				     &o->d3ddev);

	o->d3ddev->lpVtbl->SetRenderState(o->d3ddev, D3DRS_LIGHTING, FALSE);

	handle->internal = o;

	return 0;
}

static void destroy_d3d8(MwWidget handle) {
	MwWidget   w = handle;
	gdid3d8_t* o = handle->internal;

	o->d3ddev->lpVtbl->Release(o->d3ddev); // close and release the 3D device
	o->d3d->lpVtbl->Release(o->d3d);       // close and release Direct3D

	free(handle->internal);
}

static void func_handler_d3d8(MwWidget handle, const char* name, void* out,
			      va_list va) {
	gdid3d8_t* o = handle->internal;

	if(strcmp(name, "mwDirectXGetD3D8") == 0) {
		*(LPDIRECT3D8*)out = o->d3d;
	}
	if(strcmp(name, "mwDirectXGetD3Dev8") == 0) {
		*(LPDIRECT3DDEVICE8*)out = o->d3ddev;
	}
}

MwClassRec MwDX8ClassRec = {wcreate_d3d8,      /* create */
			    destroy_d3d8,      /* destroy */
			    NULL,	       /* draw */
			    NULL,	       /* click */
			    NULL,	       /* parent_resize */
			    NULL,	       /* prop_change */
			    NULL,	       /* mouse_move */
			    NULL,	       /* mouse_up */
			    NULL,	       /* mouse_down */
			    NULL,	       /* key */
			    func_handler_d3d8, /* execute */
			    NULL,	       /* tick */
			    NULL,	       /* resize */
			    NULL,	       /* children_update */
			    NULL,	       /* children_prop_change */
			    NULL,	       /* clipboard */
			    NULL,	       /* props_change */
			    NULL, NULL, NULL};
MwClass	   MwD3D8Class	 = &MwDX8ClassRec;
#else
MWDECL MwClass MwDX8Class;

MwClass MwDX8Class = NULL;
#endif

#include <Mw/Milsko.h>

#ifdef MW_DIRECT3D9
#include <Mw/Widget/Direct3D9.h>
typedef struct gdid3d9 {
	void* d3d9dll;
	IDirect3D9*(WINAPI* Direct3DCreate9)(UINT sdk_version);

	LPDIRECT3D9	  d3d;	  // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev; // the pointer to the device class

} gdid3d9_t;

#ifndef D3D_SDK_VERSION
#define D3D_SDK_VERSION 32
#endif

static int wcreate_d3d9(MwWidget handle) {
	gdid3d9_t*	      o = malloc(sizeof(gdid3d9_t));
	HRESULT		      hr;
	char		      errbuf[2048];
	D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information

	o->d3d9dll = LoadLibrary("d3d9.dll");
	if(!o->d3d9dll) {
		MessageBox(NULL, "d3d9.dll not found! Is DirectX9 installed?", "Error", MB_OK);
		ExitProcess(-1);
	}
	o->Direct3DCreate9 = (void*)GetProcAddress(o->d3d9dll, "Direct3DCreate9");

	o->d3d = o->Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
	if(o->d3d == NULL) {
		MwDispatchError(1, "Direct3DCreate9 NULL");
		return 1;
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp));		  // clear out the struct for use
	d3dpp.Windowed	    = TRUE;			  // program windowed, not fullscreen
	d3dpp.SwapEffect    = D3DSWAPEFFECT_DISCARD;	  // discard old frames
	d3dpp.hDeviceWindow = handle->lowlevel->gdi.hWnd; // set the window to be used by Direct3D

	// create a device class using this information and information from the d3dpp stuct
	hr = o->d3d->lpVtbl->CreateDevice(o->d3d,
					  D3DADAPTER_DEFAULT,
					  D3DDEVTYPE_HAL,
					  handle->lowlevel->gdi.hWnd,
					  D3DCREATE_MIXED_VERTEXPROCESSING,
					  &d3dpp,
					  &o->d3ddev);
	if(hr != D3D_OK) {
		MwStringPrintIntoBuffer(errbuf, sizeof(errbuf) - 1, "CreateDevice ERROR: %s", hr);
		MwDispatchError(1, errbuf);
		return 1;
	}

	handle->internal = o;

	return 0;
}

static void destroy_d3d9(MwWidget handle) {
	gdid3d9_t* o = handle->internal;

	o->d3ddev->lpVtbl->Release(o->d3ddev); // close and release the 3D device
	o->d3d->lpVtbl->Release(o->d3d);       // close and release Direct3D

	free(handle->internal);
}

static void func_handler_d3d9(MwWidget handle, const char* name, void* out,
			      va_list va) {
	gdid3d9_t* o = handle->internal;

	(void)va;

	if(strcmp(name, "mwDirect3D9GetDirect3D9") == 0) {
		*(LPDIRECT3D9*)out = o->d3d;
	}
	if(strcmp(name, "mwDirect3D9GetDirect3DDevice9") == 0) {
		*(LPDIRECT3DDEVICE9*)out = o->d3ddev;
	}
}

MwClassRec MwDirect3D9ClassRec = {wcreate_d3d9,	     /* create */
				  destroy_d3d9,	     /* destroy */
				  NULL,		     /* draw */
				  NULL,		     /* click */
				  NULL,		     /* parent_resize */
				  NULL,		     /* prop_change */
				  NULL,		     /* mouse_move */
				  NULL,		     /* mouse_up */
				  NULL,		     /* mouse_down */
				  NULL,		     /* key */
				  func_handler_d3d9, /* execute */
				  NULL,		     /* tick */
				  NULL,		     /* resize */
				  NULL,		     /* children_update */
				  NULL,		     /* children_prop_change */
				  NULL,		     /* clipboard */
				  NULL,		     /* props_change */
				  NULL, NULL, NULL};
MwClass	   MwDirect3D9Class    = &MwDirect3D9ClassRec;
#else
MwClass MwDirect3D9Class = NULL;
#endif

/*
 * Adapted from:
 * http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-1
 * http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-4
 */
#include <Mw/Milsko.h>
#include <Mw/Widget/D3D9.h>

MwWidget		window, d3d8;
LPDIRECT3D9		d3d;
LPDIRECT3DDEVICE9	d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer;

struct CUSTOMVERTEX {
	FLOAT x, y, z, rhw; // from the D3DFVF_XYZRHW flag
	DWORD color;	    // from the D3DFVF_DIFFUSE flag
};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

static void MWAPI draw(MwWidget handle, void* user, void* client) {
	// clear the window to a deep blue
	d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	d3ddev->lpVtbl->BeginScene(d3ddev); // begins the 3D scene

	// select which vertex format we are using
	d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);

	// select the vertex buffer to display
	d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, v_buffer, 0, sizeof(struct CUSTOMVERTEX));

	// copy the vertex buffer to the back buffer
	d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0, 1);

	d3ddev->lpVtbl->EndScene(d3ddev); // ends the 3D scene

	d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL); // displays the created frame
}

int main() {
	VOID* pVoid;

	MwLibraryInit();

	window = MwCreateWidget(MwWindowClass, NULL, NULL, MwDEFAULT, MwDEFAULT, 1024, 768);

	d3d8 = MwCreateWidget(MwD3D9Class, NULL, window, (1024 - 800) / 2, (768 - 600) / 2, 800, 600);

	d3d    = MwDirectXGetD3D9(d3d8);
	d3ddev = MwDirectXGetD3Dev9(d3d8);

	// create three vertices using the CUSTOMVERTEX struct built earlier
	struct CUSTOMVERTEX vertices[] =
	    {
		{
		    400.0f,
		    0.0f,
		    0.5f,
		    1.0f,
		    D3DCOLOR_XRGB(0, 0, 255),
		},
		{
		    800.0f,
		    600.0f,
		    0.5f,
		    1.0f,
		    D3DCOLOR_XRGB(0, 255, 0),
		},
		{
		    0.0f,
		    600.0f,
		    0.5f,
		    1.0f,
		    D3DCOLOR_XRGB(255, 0, 0),
		},
	    };

	d3ddev->lpVtbl->CreateVertexBuffer(d3ddev,
					   3 * sizeof(struct CUSTOMVERTEX),
					   0,
					   CUSTOMFVF,
					   D3DPOOL_MANAGED,
					   &v_buffer,
					   NULL);

	v_buffer->lpVtbl->Lock(v_buffer, 0, 0, (void**)&pVoid, 0); // lock the vertex buffer
	memcpy(pVoid, vertices, sizeof(vertices));		   // copy the vertices to the locked buffer
	v_buffer->lpVtbl->Unlock(v_buffer);			   // unlock the vertex buffer

	MwAddUserHandler(window, MwNtickHandler, draw, NULL);

	MwLoop(window);
}

#include "Mw/LowLevel.h"

#include "mac.h"
#include "quickDraw.h"
#include <signal.h>

MwLL MwLLCreate(MwLL parent, int x, int y, int width, int height) {
	void* library;
	MwLL  r = malloc(sizeof(*r));
	MwLLCreateCommon(r);

	library = dlopen("CarbonLib", RTLD_NOW);
	if(library != NULL) {
		dlclose(library);
		r->backend = getQuickDrawBackend();
		quickDrawBackendUserDataInit(r->userdata);
		return r;
	}

	printf("ERROR: No supported UI library found. (Searched for: CarbonLib)\n");
	getchar();
	raise(SIGTRAP);

	return NULL;
};

void MwLLSleep(int ms) {
	usleep(ms * 1000);
}

void MwLLFreeColor(MwLLColor color) {
	free(color);
}

void MwLLDestroyPixmap(MwLLPixmap pixmap) {
	free(pixmap);
}

void MwLLDestroy(MwLL handle) {
	handle->backend.destroy(handle);
};
void MwLLPolygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	handle->backend.polygon(handle, points, points_count, color);
};
MwLLColor MwLLAllocColor(MwLL handle, int r, int g, int b) {
	return handle->backend.allocColor(handle, r, g, b);
};
void MwLLGetXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	return handle->backend.getXYWH(handle, x, y, w, h);
};
void MwLLSetXY(MwLL handle, int x, int y) {
	return handle->backend.setXY(handle, x, y);
};
void MwLLSetWH(MwLL handle, int w, int h) {
	return handle->backend.setWH(handle, w, h);
};
void MwLLSetTitle(MwLL handle, const char* title) {
	return handle->backend.setTitle(handle, title);
};
int MwLLPending(MwLL handle) {
	return handle->backend.pending(handle);
};
void MwLLNextEvent(MwLL handle) {
	return handle->backend.nextEvent(handle);
};
MwLLPixmap MwLLCreatePixmap(MwLL handle, unsigned char* data, int width, int height) {
	return handle->backend.createPixmap(handle, data, width, height);
};
void MwLLDrawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	return handle->backend.drawPixmap(handle, rect, pixmap);
};
void MwLLSetIcon(MwLL handle, MwLLPixmap pixmap) {
	return handle->backend.setIcon(handle, pixmap);
};
void MwLLForceRender(MwLL handle) {
	return handle->backend.forceRender(handle);
};

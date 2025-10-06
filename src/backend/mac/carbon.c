/* $Id: carbon.c 154 2025-10-04 12:33:26Z nishi $ */

#include "mac.h"
#include "carbon.h"
#include <dlfcn.h>

void carbonBackendUserDataInit(mac_backend_userdata ud) {
}

static MwLL carbon_create(MwLL parent, int x, int y, int width, int height) {
	return NULL;
};
static void carbon_destroy(MwLL handle) {
	return;
};
static void carbon_polygon(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	return;
};
static MwLLColor carbon_allocColor(MwLL handle, int r, int g, int b) {
	return NULL;
};
static void carbon_freeColor(MwLLColor color) {
	return;
};
static void carbon_getXYWH(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	return;
};
static void carbon_setXY(MwLL handle, int x, int y) {
	return;
};
static void carbon_setWH(MwLL handle, int w, int h) {
	return;
};
static void carbon_setTitle(MwLL handle, const char* title) {
	return;
};
static int carbon_pending(MwLL handle) {
	return 0;
};
static void carbon_nextEvent(MwLL handle) {
	return;
};
static MwLLPixmap carbon_createPixmap(MwLL handle, unsigned char* data, int width, int height) {
	return NULL;
};
static void carbon_drawPixmap(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	return;
};
static void carbon_setIcon(MwLL handle, MwLLPixmap pixmap) {
	return;
};
static void carbon_forceRender(MwLL handle) {
	return;
};

static mac_backend carbon_backend = {
    .create	  = carbon_create,
    .destroy	  = carbon_destroy,
    .polygon	  = carbon_polygon,
    .allocColor	  = carbon_allocColor,
    .freeColor	  = carbon_freeColor,
    .getXYWH	  = carbon_getXYWH,
    .setXY	  = carbon_setXY,
    .setWH	  = carbon_setWH,
    .setTitle	  = carbon_setTitle,
    .pending	  = carbon_pending,
    .nextEvent	  = carbon_nextEvent,
    .createPixmap = carbon_createPixmap,
    .drawPixmap	  = carbon_drawPixmap,
    .setIcon	  = carbon_setIcon,
    .forceRender  = carbon_forceRender,
};

mac_backend getCarbonBackend(void) {
	return carbon_backend;
};

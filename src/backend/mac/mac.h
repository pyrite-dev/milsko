/* $Id$ */

#ifndef __MAC_H__
#define __MAC_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

typedef enum {
	PLATFORM_QUICKDRAW = 0,
	PLATFORM_QUARTZ,
} macPlatform;

typedef struct mac_backend_t {
	MwLL (*create)(MwLL parent, int x, int y, int width, int height);
	void (*destroy)(MwLL handle);
	void (*polygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color);
	MwLLColor (*allocColor)(MwLL handle, int r, int g, int b);
	void (*freeColor)(MwLLColor color);
	void (*getXYWH)(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h);
	void (*setXY)(MwLL handle, int x, int y);
	void (*setWH)(MwLL handle, int w, int h);
	void (*setTitle)(MwLL handle, const char* title);
	int (*pending)(MwLL handle);
	void (*nextEvent)(MwLL handle);
	MwLLPixmap (*createPixmap)(MwLL handle, unsigned char* data, int width, int height);
	void (*drawPixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap);
	void (*setIcon)(MwLL handle, MwLLPixmap pixmap);
	void (*forceRender)(MwLL handle);
} mac_backend;

typedef struct mac_backend_userdata_t* mac_backend_userdata;

struct _MwLL {
	mac_backend	     backend;
	mac_backend_userdata userdata;

	int copy_buffer;

	unsigned int width;
	unsigned int height;

	MwLLHandler handler;
};

#endif

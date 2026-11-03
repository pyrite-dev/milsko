#ifndef __MW_BACKEND_APPLE_GRAPHICS_H__
#define __MW_BACKEND_APPLE_GRAPHICS_H__

#include <Mw/Milsko.h>

typedef struct _MwLLCocoaGraphics {
	void (*create_context)(MwLL handle, int width, int height);
	void (*destroy_context)(MwLL handle);
	void (*apply_clip)(MwLL handle);
	void (*present)(MwLL handle, NSRect bounds);

	void (*polygon)(MwLL handle, MwPoint* points, int points_count, MwLLColor color);
	void (*line)(MwLL handle, MwPoint* points, MwLLColor color);

	void (*update_pixmap)(MwLLPixmap pixmap);
	void (*destroy_pixmap)(MwLLPixmap pixmap);
	void (*draw_pixmap)(MwLL handle, MwRect* rect, MwLLPixmap pixmap);
} MwLLCocoaGraphics;

int MwLLCocoaCoreGraphicsSetup(MwLLCocoaGraphics* graphics);

#endif

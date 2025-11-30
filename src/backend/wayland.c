/* $Id$ */

/**
 TODO:

 - would fit with Milsko's goal to support older compositors using wl_shell. would also suck to test because most compositors outright removed that around 2021...

 */

#include <Mw/Milsko.h>

#include "../../external/stb_ds.h"

#define WAYLAND_GET_INTERFACE(r, inter) shget(r->wayland.wl_protocol_map, inter##_interface.name)

// REGISTRY
static void new_protocol(void* data, struct wl_registry* registry,
			 MwU32 name, const char* interface,
			 MwU32 version) {
	struct _MwLLWayland* wayland = data;

	wl_setup_func* func = shget(wayland->wl_protocol_setup_map, interface);
	if(func != NULL) {
		shput(wayland->wl_protocol_map, interface, func(name, data));
	}
};

static void protocol_removed(void*		 data,
			     struct wl_registry* registry,
			     MwU32		 name) {

};

// SEAT
static void wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name) {};
static void wl_seat_capabilities(void* data, struct wl_seat* wl_seat,
				 uint32_t capabilities) {};

// XDG SHELL
void xdg_wm_base_ping(void*		  data,
		      struct xdg_wm_base* xdg_wm_base,
		      uint32_t		  serial) {
	// The compositor will send us a ping event to check that we're responsive.
	// We need to send back a pong request immediately.
	xdg_wm_base_pong(xdg_wm_base, serial);
};

// XDG SHELL SURFACE
static void wl_shell_surface_ping(void*			   data,
				  struct wl_shell_surface* wl_shell_surface,
				  uint32_t		   serial) {};
static void wl_shell_surface_configure(void*			data,
				       struct wl_shell_surface* wl_shell_surface,
				       uint32_t			edges,
				       int32_t			width,
				       int32_t			height) {};

static void wl_shell_surface_popup_done(void*			 data,
					struct wl_shell_surface* wl_shell_surface) {};

static wayland_protocol_t* wl_shm_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = NULL;
	proto->context		  = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);
	return proto;
}
static wayland_protocol_t* wl_seat_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct wl_seat_listener));

	((struct wl_seat_listener*)proto->listener)->name	  = wl_seat_name;
	((struct wl_seat_listener*)proto->listener)->capabilities = wl_seat_capabilities;

	proto->context = wl_registry_bind(wayland->registry, name, &wl_seat_interface, 1);
	wl_seat_add_listener(proto->context, proto->listener, wayland);

	return proto;
}
static wayland_protocol_t* wl_compositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = NULL;
	proto->context		  = wl_registry_bind(wayland->registry, name, &wl_compositor_interface, 1);

	return proto;
}
static wayland_protocol_t* xdg_wm_base_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct xdg_wm_base_listener));

	((struct xdg_wm_base_listener*)proto->listener)->ping = xdg_wm_base_ping;

	proto->context = wl_registry_bind(wayland->registry, name, &xdg_wm_base_interface, 1);
	xdg_wm_base_add_listener(proto->context, proto->listener, wayland);

	return proto;
}
static wayland_protocol_t* wl_shell_surface_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct wl_shell_surface_listener));

	((struct wl_shell_surface_listener*)proto->listener)->ping	 = wl_shell_surface_ping;
	((struct wl_shell_surface_listener*)proto->listener)->configure	 = wl_shell_surface_configure;
	((struct wl_shell_surface_listener*)proto->listener)->popup_done = wl_shell_surface_popup_done;

	proto->context = wl_registry_bind(wayland->registry, name, &wl_shell_surface_interface, 1);
	wl_shell_surface_add_listener(proto->context, proto->listener, wayland);

	return proto;
}

static wayland_protocol_t* wp_cursor_shape_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = NULL;

	proto->context = wl_registry_bind(wayland->registry, name, &wp_cursor_shape_manager_v1_interface, 1);

	return proto;
}

typedef struct zxdg_decoration_manager_v1_context {
	struct zxdg_decoration_manager_v1* manager;
	// struct zxdg_decoration_toplevel_v1* toplevel;
	struct zxdg_toplevel_decoration_v1* decoration;
} zxdg_decoration_manager_v1_context_t;

static wayland_protocol_t* zxdg_decoration_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t*		      proto = malloc(sizeof(wayland_protocol_t));
	zxdg_decoration_manager_v1_context_t* ctx   = malloc(sizeof(zxdg_decoration_manager_v1_context_t));
	proto->listener				    = NULL;

	ctx->manager = wl_registry_bind(wayland->registry, name, &zxdg_decoration_manager_v1_interface, 1);
	;

	proto->context = ctx;

	return proto;
}

static void xdg_toplevel_configure(void*		data,
				   struct xdg_toplevel* xdg_toplevel,
				   int32_t width, int32_t height,
				   struct wl_array* states) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;

	wl_surface_commit(self->surface);

	return;
};

static void decoration_configure(
    void* data, struct zxdg_toplevel_decoration_v1* zxdg_toplevel_decoration_v1,
    uint32_t mode) {
}

static void xdg_toplevel_close(
    void* data, struct xdg_toplevel* xdg_toplevel) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;
	// Stop running if the user requests to close the toplevel
	self->running = MwFALSE;
};

static void xdg_surface_configure(
    void* data, struct xdg_surface* xdg_surface, uint32_t serial) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;

	// The compositor configures our surface, acknowledge the configure event
	xdg_surface_ack_configure(xdg_surface, serial);

	if(self->configured) {
		// If this isn't the first configure event we've received, we already
		// have a buffer attached, so no need to do anything. Commit the
		// surface to apply the configure acknowledgement.
		wl_surface_commit(self->surface);
	}

	self->configured = MwTRUE;
}

static void shell_surface_ping(void*			data,
			       struct wl_shell_surface* shell_surface,
			       uint32_t			serial) {};
static void shell_surface_configure(
    void* data, struct wl_shell_surface* shell_surface, uint32_t edges,
    int32_t width, int32_t height) {};

static void shell_surface_popup_done(
    void* data, struct wl_shell_surface* shell_surface) {};

static void setup_callbacks(struct _MwLLWayland* wayland) {
	wayland->registry_listener.global	 = new_protocol;
	wayland->registry_listener.global_remove = protocol_removed;

#define WL_INTERFACE(interface) \
	shput(wayland->wl_protocol_setup_map, interface##_interface.name, (wl_setup_func*)interface##_setup);

	WL_INTERFACE(wl_shm);
	WL_INTERFACE(wl_seat);
	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(xdg_wm_base);
	WL_INTERFACE(wl_shell_surface);
	WL_INTERFACE(zxdg_decoration_manager_v1);
	WL_INTERFACE(wp_cursor_shape_manager_v1);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	r = malloc(sizeof(*r));
	MwLLCreateCommon(r);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendWayland;

	setup_callbacks(&r->wayland);

	// Connect to the Wayland compositor
	r->wayland.display = wl_display_connect(NULL);
	if(r->wayland.display == NULL) {
		printf("wayland: failed to create display\n");
		r->common.success = MwFALSE;
		return r;
	}

	// Obtain the wl_registry and fetch the list of globals
	r->wayland.registry = wl_display_get_registry(r->wayland.display);
	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		r->common.success = MwFALSE;
		return r;
	}

	// Check that all globals we require are available
	if(
	    WAYLAND_GET_INTERFACE(r, wl_compositor)->context == NULL ||
	    WAYLAND_GET_INTERFACE(r, xdg_wm_base)->context == NULL ||
	    WAYLAND_GET_INTERFACE(r, wl_seat)->context == NULL) {

		printf("no wl_shm, wl_compositor, wl_seat, or xdg_wm_base support\n");
		r->common.success = MwFALSE;
		return r;
	}

	// wl_log_set_handler_client(_MwLLWayland::logger);

	r->wayland.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

	// Create a wl_surface, a xdg_surface and a xdg_toplevel
	r->wayland.surface = wl_compositor_create_surface(WAYLAND_GET_INTERFACE(r, wl_compositor)->context);
	r->wayland.xdg_surface =
	    xdg_wm_base_get_xdg_surface(WAYLAND_GET_INTERFACE(r, xdg_wm_base)->context, r->wayland.surface);
	r->wayland.xdg_top_level = xdg_surface_get_toplevel(r->wayland.xdg_surface);

	// setup mandatory listeners
	r->wayland.xdg_surface_listener.configure  = xdg_surface_configure;
	r->wayland.xdg_toplevel_listener.configure = xdg_toplevel_configure;
	r->wayland.xdg_toplevel_listener.close	   = xdg_toplevel_close;

	xdg_surface_add_listener(r->wayland.xdg_surface, &r->wayland.xdg_surface_listener, r);
	xdg_toplevel_add_listener(r->wayland.xdg_top_level, &r->wayland.xdg_toplevel_listener,
				  r);

	xdg_toplevel_set_title(r->wayland.xdg_top_level, "Milsko Wayland App");
	xdg_toplevel_set_app_id(r->wayland.xdg_top_level, "Milsko Wayland App");

	// if (!mFlags.windowResizable) {
	// xdg_toplevel_set_min_size(mXDGTopLevel, mWidth, mHeight);
	// xdg_toplevel_set_max_size(mXDGTopLevel, mWidth, mHeight);
	// }

	// Perform the initial commit and wait for the first configure event
	wl_surface_commit(r->wayland.surface);
	while(wl_display_dispatch(r->wayland.display) != -1 && !r->wayland.configured) {
	}

	// setup decorations if we can
	if(WAYLAND_GET_INTERFACE(r, zxdg_decoration_manager_v1)->context != NULL) {
		zxdg_decoration_manager_v1_context_t* dec = WAYLAND_GET_INTERFACE(r, zxdg_decoration_manager_v1)->context;

		dec->decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
		    dec->manager, r->wayland.xdg_top_level);

		// zxdg_toplevel_decoration_v1_add_listener(dec->decoration,
		// dec->listener, r);

		zxdg_toplevel_decoration_v1_set_mode(
		    dec->decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
	}

	wl_surface_commit(r->wayland.surface);

	r->common.success = MwTRUE;

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	free(handle);
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	c->common.red	= r;
	c->common.blue	= b;
	c->common.green = g;

	return c;
}

static void MwLLColorUpdateImpl(MwLL handle, MwLLColor c, int r, int g, int b) {
	c->common.red	= r;
	c->common.blue	= b;
	c->common.green = g;
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	*x = 0;
	*y = 0;
	*w = 0;
	*h = 0;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
}

static void MwLLFreeColorImpl(MwLLColor color) {
}

static void MwLLSetBackgroundImpl(MwLL handle, MwLLColor color) {
}

static int MwLLPendingImpl(MwLL handle) {
	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));

	r->common.width	 = width;
	r->common.height = height;
	r->common.raw	 = data;
	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
}

static void MwLLForceRenderImpl(MwLL handle) {
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
}

static void MwLLShowImpl(MwLL handle, int show) {
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
}

static void MwLLFocusImpl(MwLL handle) {
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
}

static char* MwLLGetClipboardImpl(MwLL handle) {
	char* r = NULL;
	return r;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 0);
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	MwLLShow(handle, 1);
}

static int MwLLWaylandCallInitImpl(void) {
#ifdef __linux__
	if(strcmp(getenv("XDG_SESSION_TYPE"), "wayland")) {
		return 1;
	}
#endif
	return 0;
}

#include "call.c"
CALL(Wayland);

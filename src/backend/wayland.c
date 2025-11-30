/* $Id$ */

#include <Mw/Milsko.h>

#include <fcntl.h>
#include <errno.h>
#include <poll.h>

#include "../../external/stb_ds.h"

#define WAYLAND_GET_INTERFACE(r, inter) shget(r.wl_protocol_map, inter##_interface.name)

static void new_protocol(void* data, struct wl_registry* registry,
			 MwU32 name, const char* interface,
			 MwU32 version) {
	struct _MwLLWayland* wayland = data;

	wl_setup_func* func = shget(wayland->wl_protocol_setup_map, interface);
	if(func != NULL) {
		// printf("registering interface %s\n", interface);
		shput(wayland->wl_protocol_map, interface, func(name, data));
	} else {
		// printf("unknown interface %s\n", interface);
	}
};

static void protocol_removed(void*		 data,
			     struct wl_registry* registry,
			     MwU32		 name) {

};

static void wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name) {};
static void wl_seat_capabilities(void* data, struct wl_seat* wl_seat,
				 MwU32 capabilities) {};

void xdg_wm_base_ping(void*		  data,
		      struct xdg_wm_base* xdg_wm_base,
		      MwU32		  serial) {
	// The compositor will send us a ping event to check that we're responsive.
	// We need to send back a pong request immediately.
	xdg_wm_base_pong(xdg_wm_base, serial);
};

static wayland_protocol_t* wl_shm_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->shm = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);
	return NULL;
}
static void buffer_setup(struct _MwLLWayland* wayland) {
	struct wl_shm_pool* pool;
	struct wl_buffer*   buffer;
	int		    x, y;

	int   fd;
	MwU32 size	  = wayland->ww * wayland->wh * 4;
	char  temp_name[] = "/tmp/milsko-wl-shm-XXXXXX";

	fd = mkstemp(temp_name);

	unlink(temp_name);

	if(posix_fallocate(fd, 0, size) != 0) {
		printf("failure setting up wl_shm: could not fallocate. %s.\n", strerror(errno));
		close(fd);
		return;
	}
	if(ftruncate(fd, size) != 0) {
		printf("failure setting up wl_shm: could not truncate. %s.\n", strerror(errno));
		close(fd);
		return;
	}

	for(y = 0; y < wayland->wh; y++) {
		for(x = 0; x < wayland->ww; x++) {
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
			MwU8 a = 0xFF;
			MwU8 r = MIN(((wayland->ww - x) * 0xFF) / wayland->ww, ((wayland->wh - y) * 0xFF) / wayland->wh);
			MwU8 g = MIN((x * 0xFF) / wayland->ww, ((wayland->wh - y) * 0xFF) / wayland->wh);
			MwU8 b = MIN(((wayland->ww - x) * 0xFF) / wayland->ww, (y * 0xFF) / wayland->wh);
			write(fd, &b, 1);
			write(fd, &g, 1);
			write(fd, &r, 1);
			write(fd, &a, 1);
#undef MIN
		}
	}

	fsync(fd);

	if(!(pool = wl_shm_create_pool(wayland->shm, fd, size))) {
		printf("failure setting up wl_shm: could not create pool.\n");
	}

	buffer = wl_shm_pool_create_buffer(pool, 0, wayland->ww, wayland->wh, wayland->ww * 4, WL_SHM_FORMAT_ARGB8888);

	if(wayland->configured) {
		wl_surface_attach(wayland->surface, buffer, 0, 0);
		wl_surface_commit(wayland->surface);
	}

	wl_buffer_destroy(buffer);
	close(fd);
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
	wayland->compositor = wl_registry_bind(wayland->registry, name, &wl_compositor_interface, 1);

	return NULL;
}

static wayland_protocol_t* wl_subcompositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->subcompositor = wl_registry_bind(wayland->registry, name, &wl_subcompositor_interface, 1);

	return NULL;
}

static wayland_protocol_t* xdg_wm_base_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct xdg_wm_base_listener));

	((struct xdg_wm_base_listener*)proto->listener)->ping = xdg_wm_base_ping;

	proto->context = wl_registry_bind(wayland->registry, name, &xdg_wm_base_interface, 1);
	xdg_wm_base_add_listener(proto->context, proto->listener, wayland);

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
				   MwI32 width, MwI32 height,
				   struct wl_array* states) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;

	wl_surface_commit(self->surface);

	return;
};

static void decoration_configure(
    void* data, struct zxdg_toplevel_decoration_v1* zxdg_toplevel_decoration_v1,
    MwU32 mode) {
}

static void xdg_toplevel_close(
    void* data, struct xdg_toplevel* xdg_toplevel) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;
	// Stop running if the user requests to close the toplevel
	self->toplevel.running = MwFALSE;
};

static void xdg_surface_configure(
    void* data, struct xdg_surface* xdg_surface, MwU32 serial) {
	struct _MwLLWayland* self = (struct _MwLLWayland*)data;

	// The compositor configures our surface, acknowledge the configure event
	xdg_surface_ack_configure(xdg_surface, serial);

	if(self->configured) {
		// If this isn't the first configure event we've received, we already
		// have a buffer attached, so no need to do anything. Commit the
		// surface to apply the configure acknowledgement.
		// wl_surface_attach(self->surface, self->buffer, 0, 0);
		wl_surface_commit(self->surface);
	}

	self->configured = MwTRUE;
}

#define WL_INTERFACE(interface) \
	shput(wayland->wl_protocol_setup_map, interface##_interface.name, (wl_setup_func*)interface##_setup);

static void setup_callbacks(struct _MwLLWayland* wayland) {
	wayland->registry_listener.global	 = new_protocol;
	wayland->registry_listener.global_remove = protocol_removed;

	WL_INTERFACE(wl_shm);
	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(wl_subcompositor);
	if(wayland->type == MWLL_WAYLAND_TOPLEVEL) {
		WL_INTERFACE(wl_seat);
		WL_INTERFACE(xdg_wm_base);
		WL_INTERFACE(zxdg_decoration_manager_v1);
		WL_INTERFACE(wp_cursor_shape_manager_v1);
	}
}

#undef WL_INTERFACE

static MwBool flush_display(struct wl_display* display) {
	while(wl_display_flush(display) == -1) {
		if(errno != EAGAIN) {
			return MwFALSE;
		}

		struct pollfd fd = {wl_display_get_fd(display), POLL_OUT};

		while(poll(&fd, 1, -1) == -1) {
			if(errno != EINTR && errno != EAGAIN)
				return MwFALSE;
		}
	}
	return MwTRUE;
}

static void event_loop(MwLL handle) {
	enum {
		DISPLAY_FD,
		KEYREPEAT_FD,
		CURSOR_FD,
		LIBDECOR_FD
	};
	if(handle->wayland.display == NULL) {
		return;
	}
	struct pollfd fd      = {wl_display_get_fd(handle->wayland.display), POLLIN};
	int	      timeout = 500;

	while(wl_display_prepare_read(handle->wayland.display) != 0) {
		if(wl_display_dispatch_pending(handle->wayland.display) > 0) {
			return;
		}
	}

	// If an error other than EAGAIN happens, we have likely been disconnected
	// from the Wayland session
	if(!flush_display(handle->wayland.display)) {
		wl_display_cancel_read(handle->wayland.display);

		handle->wayland.toplevel.running = MwFALSE;
		return;
	}

	if(!poll(&fd, 1, timeout)) {
		wl_display_cancel_read(handle->wayland.display);
		return;
	}

	if(fd.revents & POLLIN) {
		wl_display_read_events(handle->wayland.display);
		if(wl_display_dispatch_pending(handle->wayland.display) > 0) {
		}
	} else {
		wl_display_cancel_read(handle->wayland.display);
	}
}

static void setup_toplevel(MwLL r, int x, int y, int width, int height) {
	r->wayland.type = MWLL_WAYLAND_TOPLEVEL;

	setup_callbacks(&r->wayland);

	// Connect to the Wayland compositor
	r->wayland.display = wl_display_connect(NULL);
	if(r->wayland.display == NULL) {
		printf("wayland: failed to create display\n");
		r->common.success = MwFALSE;
		return;
	}

	// Do a roundtrip to ensure all interfaces are setup.
	r->wayland.registry = wl_display_get_registry(r->wayland.display);
	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		r->common.success = MwFALSE;
		return;
	}

	// Check that all globals we require are available
	if(
	    r->wayland.compositor == NULL ||
	    WAYLAND_GET_INTERFACE(r->wayland, xdg_wm_base)->context == NULL ||
	    WAYLAND_GET_INTERFACE(r->wayland, wl_seat)->context == NULL) {

		printf("no wl_shm, wl_compositor, wl_seat, or xdg_wm_base support\n");
		r->common.success = MwFALSE;
		return;
	}

	r->wayland.toplevel.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

	// Create a wl_surface, a xdg_surface and a xdg_toplevel
	r->wayland.surface = wl_compositor_create_surface(r->wayland.compositor);
	r->wayland.toplevel.xdg_surface =
	    xdg_wm_base_get_xdg_surface(WAYLAND_GET_INTERFACE(r->wayland, xdg_wm_base)->context, r->wayland.surface);
	r->wayland.toplevel.xdg_top_level = xdg_surface_get_toplevel(r->wayland.toplevel.xdg_surface);

	// setup mandatory listeners
	r->wayland.toplevel.xdg_surface_listener.configure  = xdg_surface_configure;
	r->wayland.toplevel.xdg_toplevel_listener.configure = xdg_toplevel_configure;
	r->wayland.toplevel.xdg_toplevel_listener.close	    = xdg_toplevel_close;

	xdg_surface_add_listener(r->wayland.toplevel.xdg_surface, &r->wayland.toplevel.xdg_surface_listener, r);
	xdg_toplevel_add_listener(r->wayland.toplevel.xdg_top_level, &r->wayland.toplevel.xdg_toplevel_listener,
				  r);

	xdg_toplevel_set_title(r->wayland.toplevel.xdg_top_level, "Milsko Wayland App");
	xdg_toplevel_set_app_id(r->wayland.toplevel.xdg_top_level, "MilskoWaylandApp");

	// Perform the initial commit and wait for the first configure event
	wl_surface_commit(r->wayland.surface);
	while(wl_display_dispatch(r->wayland.display) != -1 && !r->wayland.configured) {
	}

	// setup decorations if we can
	if(WAYLAND_GET_INTERFACE(r->wayland, zxdg_decoration_manager_v1)->context != NULL) {
		zxdg_decoration_manager_v1_context_t* dec = WAYLAND_GET_INTERFACE(r->wayland, zxdg_decoration_manager_v1)->context;

		dec->decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
		    dec->manager, r->wayland.toplevel.xdg_top_level);

		zxdg_toplevel_decoration_v1_set_mode(
		    dec->decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
	} else {
		printf("zxdg null\n");
	}

	buffer_setup(&r->wayland);

	wl_surface_commit(r->wayland.surface);
}

static void setup_subsurface(MwLL parent, MwLL r, int x, int y, int width, int height) {
	struct wl_compositor*	 compositor	= parent->wayland.compositor;
	struct wl_subcompositor* subcompositor	= parent->wayland.subcompositor;
	struct wl_surface*	 parent_surface = parent->wayland.surface;

	r->wayland.type = MWLL_WAYLAND_SUBSURFACE;

	setup_callbacks(&r->wayland);

	r->wayland.registry = wl_display_get_registry(parent->wayland.display);
	r->wayland.display  = parent->wayland.display;

	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}
	r->wayland.display = parent->wayland.display;

	r->wayland.surface = wl_compositor_create_surface(compositor);

	r->wayland.subsurface = wl_subcompositor_get_subsurface(subcompositor, r->wayland.surface, parent_surface);

	wl_subsurface_set_position(r->wayland.subsurface, x, y);

	// printf("position %d %d\n", x, y);

	r->wayland.configured = MwTRUE;

	buffer_setup(&r->wayland);

	wl_surface_commit(parent->wayland.surface);
}
static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	r = malloc(sizeof(*r));
	MwLLCreateCommon(r);

	r->common.copy_buffer = 1;
	r->common.type	      = MwLLBackendWayland;

	r->wayland.ww = width;
	r->wayland.wh = height;

	r->common.success = MwTRUE;

	if(parent == NULL) {
		setup_toplevel(r, x, y, width, height);
	} else {
		setup_subsurface(parent, r, x, y, width, height);
	}

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
	event_loop(handle);
	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	event_loop(handle);
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		xdg_toplevel_set_title(handle->wayland.toplevel.xdg_top_level, title);
	}
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
	// event_loop(handle);
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

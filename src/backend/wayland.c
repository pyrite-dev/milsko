#include <Mw/Milsko.h>

#include <poll.h>
#include <wayland-client-protocol.h>

#include "../../external/stb_ds.h"

#include <sys/mman.h>
#include <wayland-util.h>

/* TODO: find out what FreeBSD and such wants us to include */
#ifdef __FreeBSD__
/* XXX: Untested (nishi) */
#include <evdev/input.h>
#else
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

/* Setup the wl_shm buffer with the saved width/height */
static void buffer_setup(struct _MwLLWayland* wayland);
/* Destroy the wl_shm buffer */
static void buffer_destroy(struct _MwLLWayland* handle);

static void region_setup(MwLL handle);
static void region_invalidate(MwLL handle);

/* Get the registered interface from r, or NULL if it doesn't currently have it. */
#define WAYLAND_GET_INTERFACE(handle, inter) shget(handle.wl_protocol_map, inter##_interface.name)

/* `wl_registry.global` callback */
static void new_protocol(void* data, struct wl_registry* registry,
			 MwU32 name, const char* interface,
			 MwU32 version) {
	struct _MwLLWayland* wayland = data;

	wl_setup_func* func = shget(wayland->wl_protocol_setup_map, interface);
	if(func != NULL) {
		/* printf("registering interface %s\n", interface); */
		shput(wayland->wl_protocol_map, interface, func(name, data));
	} else {
		/* printf("unknown interface %s\n", interface); */
	}
};

/* `wl_registry.global_remove` callback */
static void protocol_removed(void*		 data,
			     struct wl_registry* registry,
			     MwU32		 name) {

};

/* `wl_pointer.enter` callback */
static void pointer_enter(void* data, struct wl_pointer* wl_pointer, MwU32 serial,
			  struct wl_surface* surface, wl_fixed_t surface_x,
			  wl_fixed_t surface_y) {
};

/* `wl_pointer.leave` callback */
static void pointer_leave(void* data, struct wl_pointer* wl_pointer, MwU32 serial,
			  struct wl_surface* surface) {
};

/* `wl_pointer.motion` callback */
static void pointer_motion(void* data, struct wl_pointer* wl_pointer, MwU32 time,
			   wl_fixed_t surface_x, wl_fixed_t surface_y) {
	MwLL	  self = data;
	MwLLMouse p;

	self->wayland.cur_mouse_pos.x = wl_fixed_to_double(surface_x);
	self->wayland.cur_mouse_pos.y = wl_fixed_to_double(surface_y);

	p.point = self->wayland.cur_mouse_pos;
	MwLLDispatch(self, move, &p);

	/*timed_redraw(self, time, 50, &self->wayland.cooldown_timer);*/
};

/* `wl_pointer.button` callback */
static void pointer_button(void* data, struct wl_pointer* wl_pointer, MwU32 serial, MwU32 time, MwU32 button, MwU32 state) {
	MwLL	  self = data;
	MwLLMouse p;
	p.point = self->wayland.cur_mouse_pos;
	if(p.point.x > self->wayland.x && p.point.x < self->wayland.x + self->wayland.ww && p.point.y > self->wayland.y && p.point.y < self->wayland.y + self->wayland.wh) {
		switch(button) {
		case BTN_LEFT:
			p.button = MwLLMouseLeft;
			break;
		case BTN_MIDDLE:
			p.button = MwLLMouseMiddle;
			break;
		case BTN_RIGHT:
			p.button = MwLLMouseRight;
			break;
		}
		switch(state) {
		case WL_POINTER_BUTTON_STATE_PRESSED:
			MwLLDispatch(self, down, &p);
			break;
		case WL_POINTER_BUTTON_STATE_RELEASED:
			MwLLDispatch(self, up, &p);
			break;
		}
	}

	MwLLDispatch(self, draw, NULL);
};

/* `wl_pointer.axis` callback */
static void pointer_axis(void* data, struct wl_pointer* wl_pointer, MwU32 time,
			 MwU32 axis, wl_fixed_t value) {};

struct wl_pointer_listener pointer_listener = {
    .enter  = pointer_enter,
    .leave  = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis   = pointer_axis,
};

/* Recursively dispatch the key event to `handle` and its children */
static void recursive_key(MwLL handle, void* ud) {
	int i;
	if(handle->wayland.sublevels != NULL) {
		for(i = 0; i < arrlen(handle->wayland.sublevels); i++) {
			MwLLDispatch(handle->wayland.sublevels[i], key, ud);
			recursive_key(handle->wayland.sublevels[i], ud);
		}
	}
}

/* Recursively dispatch the key_released event to `handle` and its children */
static void recursive_key_released(MwLL handle, void* ud) {
	int i;
	if(handle->wayland.sublevels != NULL) {
		for(i = 0; i < arrlen(handle->wayland.sublevels); i++) {
			MwLLDispatch(handle->wayland.sublevels[i], key_released, ud);
			recursive_key(handle->wayland.sublevels[i], ud);
		}
	}
}

/* `wl_keyboard.keymap` callback */
static void keyboard_keymap(void*		data,
			    struct wl_keyboard* wl_keyboard,
			    MwU32		format,
			    int32_t		fd,
			    MwU32		size) {
	MwLL self = data;
	if(self->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		struct _MwLLWaylandTopLevel* wayland = self->wayland.toplevel;
		assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

		char* map_shm = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
		assert(map_shm != MAP_FAILED);

		struct xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(
		    wayland->xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1,
		    XKB_KEYMAP_COMPILE_NO_FLAGS);
		munmap(map_shm, size);
		close(fd);

		struct xkb_state* xkb_state = xkb_state_new(xkb_keymap);
		xkb_keymap_unref(wayland->xkb_keymap);
		xkb_state_unref(wayland->xkb_state);

		wayland->xkb_keymap = xkb_keymap;
		wayland->xkb_state  = xkb_state;
	}
};

/* `wl_keyboard.enter` callback */
static void keyboard_enter(void*	       data,
			   struct wl_keyboard* wl_keyboard,
			   MwU32	       serial,
			   struct wl_surface*  surface,
			   struct wl_array*    keys) {
	MwLL self = data;
	MwLLDispatch(self, focus_in, NULL);
};

/* `wl_keyboard.leave` callback */
static void keyboard_leave(void*	       data,
			   struct wl_keyboard* wl_keyboard,
			   MwU32	       serial,
			   struct wl_surface*  surface) {
	MwLL self = data;
	MwLLDispatch(self, focus_out, NULL);
};

/* `wl_keyboard.key` callback */
static void keyboard_key(void*		     data,
			 struct wl_keyboard* wl_keyboard,
			 MwU32		     serial,
			 MwU32		     time,
			 MwU32		     key,
			 MwU32		     state) {
	MwLL self = data;
	if(self->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		struct _MwLLWaylandTopLevel* wayland = self->wayland.toplevel;
		xkb_layout_index_t	     layout;
		xkb_level_index_t	     level;
		const xkb_keysym_t*	     syms_out;
		MwU32			     keycode = key + 8;
		MwU64			     syms_num;
		int			     i;

		if(!wayland->xkb_keymap) {
			return;
		}

		layout = xkb_state_key_get_layout(wayland->xkb_state, keycode);
		level =
		    xkb_keymap_num_levels_for_key(wayland->xkb_keymap, keycode, layout) - 1;
		syms_num = xkb_keymap_key_get_syms_by_level(wayland->xkb_keymap, keycode, layout, level, &syms_out);
		if(syms_out == NULL) {
			return;
		}

		for(i = 0; i < syms_num; i++) {
			int sym = syms_out[i];
			int key;
			switch(sym) {
			case XKB_KEY_BackSpace:
				key = MwLLKeyBackSpace;
				break;
			case XKB_KEY_Left:
				key = MwLLKeyLeft;
				break;
			case XKB_KEY_Right:
				key = MwLLKeyRight;
				break;
			case XKB_KEY_Up:
				key = MwLLKeyUp;
				break;
			case XKB_KEY_Down:
				key = MwLLKeyDown;
				break;
			case XKB_KEY_Return:
				key = MwLLKeyEnter;
				break;
			case XKB_KEY_Escape:
				key = MwLLKeyEscape;
				break;
			case XKB_KEY_Shift_L:
				key = MwLLKeyLeftShift;
				break;
			case XKB_KEY_Shift_R:
				key = MwLLKeyRightShift;
				break;
			case XKB_KEY_Alt_L:
			case XKB_KEY_Alt_R:
				key = MwLLKeyAlt;
				break;
			case XKB_KEY_Control_L:
			case XKB_KEY_Control_R:
				key = MwLLKeyControl;
				break;
			default:
				key = sym;
			}
			if(state == WL_KEYBOARD_KEY_STATE_PRESSED) {
				MwLLDispatch(self, key, &key);
				recursive_key(self, &key);
			} else {
				MwLLDispatch(self, key_released, &key);
				recursive_key_released(self, &key);
			}
		}
	}
};

/* `wl_keyboard.modifiers` callback */
static void keyboard_modifiers(void*		   data,
			       struct wl_keyboard* wl_keyboard,
			       MwU32		   serial,
			       MwU32		   mods_depressed,
			       MwU32		   mods_latched,
			       MwU32		   mods_locked,
			       MwU32		   group) {};

struct wl_keyboard_listener keyboard_listener = {
    .keymap    = keyboard_keymap,
    .enter     = keyboard_enter,
    .leave     = keyboard_leave,
    .key       = keyboard_key,
    .modifiers = keyboard_modifiers,
};

/* `wl_seat.name` callback */
static void
wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name) {};

/* `wl_seat.capabilities` callback */
static void wl_seat_capabilities(void* data, struct wl_seat* wl_seat,
				 MwU32 capabilities) {
	MwLL self = data;
	if(capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
		struct wl_keyboard* keyboard = wl_seat_get_keyboard(wl_seat);
		wl_keyboard_add_listener(keyboard, &keyboard_listener, data);
	}
	if(capabilities & WL_SEAT_CAPABILITY_POINTER) {
		struct wl_pointer* pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(pointer, &pointer_listener, data);
	}
};

/* `xdg_wm_base.ping` callback */
void xdg_wm_base_ping(void*		  data,
		      struct xdg_wm_base* xdg_wm_base,
		      MwU32		  serial) {
	xdg_wm_base_pong(xdg_wm_base, serial);
};

/* wl_seat setup function */
static wayland_protocol_t* wl_seat_setup(MwU32 name, MwLL ll) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct wl_seat_listener));

	((struct wl_seat_listener*)proto->listener)->name	  = wl_seat_name;
	((struct wl_seat_listener*)proto->listener)->capabilities = wl_seat_capabilities;

	proto->context = wl_registry_bind(ll->wayland.registry, name, &wl_seat_interface, 1);
	wl_seat_add_listener(proto->context, proto->listener, ll);

	return proto;
}

/* wl_compositor setup function */
static wayland_protocol_t* wl_compositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->compositor = wl_registry_bind(wayland->registry, name, &wl_compositor_interface, 1);

	return NULL;
}

/* wl_subcompositor setup function */
static wayland_protocol_t* wl_subcompositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->sublevel->subcompositor = wl_registry_bind(wayland->registry, name, &wl_subcompositor_interface, 1);

	return NULL;
}

/* xdg_wm_base setup function */
static wayland_protocol_t* xdg_wm_base_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = malloc(sizeof(struct xdg_wm_base_listener));

	((struct xdg_wm_base_listener*)proto->listener)->ping = xdg_wm_base_ping;

	proto->context = wl_registry_bind(wayland->registry, name, &xdg_wm_base_interface, 1);
	xdg_wm_base_add_listener(proto->context, proto->listener, wayland);

	return proto;
}

/* wp_cursor_shape_manager setup function */
static wayland_protocol_t* wp_cursor_shape_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	proto->listener		  = NULL;

	proto->context = wl_registry_bind(wayland->registry, name, &wp_cursor_shape_manager_v1_interface, 1);

	return proto;
}

/* the two decoration manager constructs */
typedef struct zxdg_decoration_manager_v1_context {
	struct zxdg_decoration_manager_v1*  manager;
	struct zxdg_toplevel_decoration_v1* decoration;
} zxdg_decoration_manager_v1_context_t;

/* zxdg_decoration_manager_v1 setup function */
static wayland_protocol_t* zxdg_decoration_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t*		      proto = malloc(sizeof(wayland_protocol_t));
	zxdg_decoration_manager_v1_context_t* ctx   = malloc(sizeof(zxdg_decoration_manager_v1_context_t));
	proto->listener				    = NULL;

	ctx->manager = wl_registry_bind(wayland->registry, name, &zxdg_decoration_manager_v1_interface, 1);
	;

	proto->context = ctx;

	return proto;
}

/* `xdg_toplevel.close` callback */
static void xdg_toplevel_configure(void*		data,
				   struct xdg_toplevel* xdg_toplevel,
				   MwI32 width, MwI32 height,
				   struct wl_array* states) {
	MwLL self = data;
	int  i;

	if(width == 0 || height == 0) {
		width  = self->wayland.ww;
		height = self->wayland.wh;
		/* if it's still 0 then bail */
		if(width == 0 || height == 0) {
			return;
		}
	}

	region_invalidate(self);
	self->wayland.ww = width;
	self->wayland.wh = height;
	xdg_surface_set_window_geometry(self->wayland.toplevel->xdg_surface, 0, 0, self->wayland.ww, self->wayland.wh);

	buffer_destroy(&self->wayland);
	buffer_setup(&self->wayland);
	region_setup(self);

	MwLLDispatch(self, resize, NULL);
	MwLLDispatch(self, draw, NULL);

	for(i = 0; i < arrlen(self->wayland.sublevels); i++) {
		MwLL handle = self->wayland.sublevels[i];
		region_setup(handle);
	}

	/*if(!self->wayland.egl_setup) {
		self->wayland.egl_setup = egl_setup(self, self->wayland.x, self->wayland.y, width, height);
	} else {
		wl_egl_window_resize(self->wayland.egl_window_native, width, height, 0, 0);
		egl_resetup(self);
	}*/

	return;
};

/* Empty function for satisfying zxdg_toplevel's requirements */
static void decoration_configure(
    void* data, struct zxdg_toplevel_decoration_v1* zxdg_toplevel_decoration_v1,
    MwU32 mode) {
}

/* `xdg_surface.close` callback */
static void xdg_toplevel_close(
    void* data, struct xdg_toplevel* xdg_toplevel) {
	MwLL self = data;
	MwLLDispatch(self, close, NULL);
};

/* `xdg_surface.configure` callback */
static void xdg_surface_configure(
    void* data, struct xdg_surface* xdg_surface, MwU32 serial) {
	MwLL self = data;

	xdg_surface_ack_configure(xdg_surface, serial);

	if(self->wayland.configured) {
		wl_surface_commit(self->wayland.surface);
	}

	self->wayland.configured = MwTRUE;
}

/* wl_shm setup function */
static wayland_protocol_t* wl_shm_setup(MwU32 name, struct _MwLLWayland* wayland) {

	wayland->shm = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);

	return NULL;
}

static void update_buffer(MwLL handle) {
	fsync(handle->wayland.shm_fd);

	if(handle->wayland.configured) {
		wl_surface_attach(handle->wayland.surface, handle->wayland.shm_buffer, 0, 0);
		wl_surface_commit(handle->wayland.surface);
	}
}

static void buffer_setup(struct _MwLLWayland* wayland) {
	int  x, y;
	int  stride	 = wayland->ww * 4;
	char temp_name[] = "/tmp/milsko-wl-shm-XXXXXX";

	wayland->mapped_shm_buf_size = wayland->ww * wayland->wh * 4;

	wayland->shm_fd = mkstemp(temp_name);

	unlink(temp_name);

	if(posix_fallocate(wayland->shm_fd, 0, wayland->mapped_shm_buf_size) != 0) {
		printf("failure setting up wl_shm: could not fallocate. %s.\n", strerror(errno));
		close(wayland->shm_fd);
		return;
	}
	if(ftruncate(wayland->shm_fd, wayland->mapped_shm_buf_size) != 0) {
		printf("failure setting up wl_shm: could not truncate. %s.\n", strerror(errno));
		close(wayland->shm_fd);
		return;
	}

	wayland->mapped_shm_buf = mmap(NULL, wayland->mapped_shm_buf_size, PROT_WRITE, MAP_SHARED, wayland->shm_fd, 0);

	fsync(wayland->shm_fd);

	if(!(wayland->shm_pool = wl_shm_create_pool(wayland->shm, wayland->shm_fd, wayland->mapped_shm_buf_size))) {
		printf("failure setting up wl_shm: could not create pool.\n");
	}

	wayland->shm_buffer = wl_shm_pool_create_buffer(wayland->shm_pool, 0, wayland->ww, wayland->wh, stride, WL_SHM_FORMAT_ARGB8888);

	if(wayland->configured) {
		wl_surface_attach(wayland->surface, wayland->shm_buffer, 0, 0);
		wl_surface_commit(wayland->surface);
	}

	wayland->cs    = cairo_image_surface_create_for_data(wayland->mapped_shm_buf, CAIRO_FORMAT_ARGB32, wayland->ww, wayland->wh, 4 * wayland->ww);
	wayland->cairo = cairo_create(wayland->cs);

	memset(wayland->mapped_shm_buf, 255, wayland->mapped_shm_buf_size);
	update_buffer((MwLL)wayland);
}

static void buffer_destroy(struct _MwLLWayland* wayland) {
	if(!wayland->configured) {
		return;
	}
	cairo_destroy(wayland->cairo);
	cairo_surface_destroy(wayland->cs);

	wl_buffer_destroy(wayland->shm_buffer);
	// munmap(wayland->mapped_shm_buf, wayland->mapped_shm_buf_size);
	wl_shm_pool_destroy(wayland->shm_pool);
	close(wayland->shm_fd);
}

static void region_invalidate(MwLL handle) {
	if(!handle->wayland.configured) {
		return;
	}
	wl_region_subtract(handle->wayland.region, handle->wayland.x, handle->wayland.y, handle->wayland.ww, handle->wayland.wh);
}
static void region_setup(MwLL handle) {
	if(!handle->wayland.configured) {
		return;
	}
	wl_region_add(handle->wayland.region, handle->wayland.x, handle->wayland.y, handle->wayland.ww, handle->wayland.wh);
	wl_surface_set_input_region(handle->wayland.surface, handle->wayland.region);
	wl_surface_set_opaque_region(handle->wayland.surface, handle->wayland.region);
}

/* Standard Wayland event loop. */
static int event_loop(MwLL handle) {
	enum {
		DISPLAY_FD,
		KEYREPEAT_FD,
		CURSOR_FD
	};
	struct pollfd	     fd;
	int		     timeout = 1;
	struct _MwLLWayland* wayland = &handle->wayland;

	if(wayland->display == NULL) {
		return 0;
	}
	fd.fd	  = wl_display_get_fd(wayland->display);
	fd.events = POLLIN;

	while(wl_display_prepare_read(handle->wayland.display) != 0) {
		if(wl_display_dispatch_pending(handle->wayland.display) > 0) {
			return 0;
		}
	}

	/* If an error other than EAGAIN happens, we have likely been disconnected from the Wayland session */
	while(wl_display_flush(wayland->display) == -1) {
		if(errno != EAGAIN) {
			return MwFALSE;
		}

		while(poll(&fd, 1, -1) == -1) {
			if(errno != EINTR && errno != EAGAIN) {
				wl_display_cancel_read(wayland->display);

				MwLLDispatch(handle, close, NULL);
				return 0;
			}
		}
	}

	/* Condition where no events are being sent. */
	if(!poll(&fd, 1, timeout)) {
		wl_display_cancel_read(handle->wayland.display);
		/* In this case, we need to commit the surface for any animations, etc. */
		wl_surface_commit(handle->wayland.surface);
		return 0;
	}

	if(fd.revents & POLLIN) {
		wl_display_read_events(wayland->display);
		if(wl_display_dispatch_pending(wayland->display) > 0) {
		}
	} else {
		wl_display_cancel_read(handle->wayland.display);
	}

	return 1;
}

/* Function for setting up the callbacks/structs that will be registered upon the relevant interfaces being found. */
static void setup_callbacks(struct _MwLLWayland* wayland) {
/* Convience macro for adding the interface functions to the setup map */
#define WL_INTERFACE(interface) \
	shput(wayland->wl_protocol_setup_map, interface##_interface.name, (wl_setup_func*)interface##_setup);

	wayland->registry_listener.global	 = new_protocol;
	wayland->registry_listener.global_remove = protocol_removed;
	wayland->wl_protocol_setup_map		 = NULL;
	wayland->wl_protocol_map		 = NULL;

	WL_INTERFACE(wl_shm);
	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(wl_seat);
	if(wayland->type == MWLL_WAYLAND_TOPLEVEL) {
		WL_INTERFACE(xdg_wm_base);
		WL_INTERFACE(zxdg_decoration_manager_v1);
		WL_INTERFACE(wp_cursor_shape_manager_v1);
	} else {
		WL_INTERFACE(wl_subcompositor);
	}
#undef WL_INTERFACE
}

/* Toplevel setup function */
static void setup_toplevel(MwLL r, int x, int y) {
	int i;

	r->wayland.type	    = MWLL_WAYLAND_TOPLEVEL;
	r->wayland.toplevel = malloc(sizeof(struct _MwLLWaylandTopLevel));

	setup_callbacks(&r->wayland);

	/* Connect to the Wayland compositor */
	r->wayland.display = wl_display_connect(NULL);
	if(r->wayland.display == NULL) {
		printf("wayland: failed to create display\n");
		raise(SIGTRAP);
		return;
	}

	/* Do a roundtrip to ensure all interfaces are setup. */
	r->wayland.registry = wl_display_get_registry(r->wayland.display);
	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}

	r->wayland.toplevel->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);

	/* Create a wl_surface, a xdg_surface and a xdg_toplevel */
	r->wayland.surface = wl_compositor_create_surface(r->wayland.compositor);
	r->wayland.toplevel->xdg_surface =
	    xdg_wm_base_get_xdg_surface(WAYLAND_GET_INTERFACE(r->wayland, xdg_wm_base)->context, r->wayland.surface);
	r->wayland.toplevel->xdg_top_level = xdg_surface_get_toplevel(r->wayland.toplevel->xdg_surface);

	/* setup mandatory listeners */
	r->wayland.toplevel->xdg_surface_listener.configure = xdg_surface_configure;

	r->wayland.toplevel->xdg_toplevel_listener.configure = xdg_toplevel_configure;
	r->wayland.toplevel->xdg_toplevel_listener.close     = xdg_toplevel_close;

	xdg_surface_add_listener(r->wayland.toplevel->xdg_surface, &r->wayland.toplevel->xdg_surface_listener, r);
	xdg_toplevel_add_listener(r->wayland.toplevel->xdg_top_level, &r->wayland.toplevel->xdg_toplevel_listener,
				  r);

	xdg_toplevel_set_title(r->wayland.toplevel->xdg_top_level, "Milsko App");
	xdg_toplevel_set_app_id(r->wayland.toplevel->xdg_top_level, "MilskoWaylandApp");

	/* Perform the initial commit and wait for the first configure event */
	wl_surface_commit(r->wayland.surface);
	event_loop(r);

	/* setup decorations if we can */
	if(shget(r->wayland.wl_protocol_map, zxdg_decoration_manager_v1_interface.name) != NULL) {
		zxdg_decoration_manager_v1_context_t* dec = WAYLAND_GET_INTERFACE(r->wayland, zxdg_decoration_manager_v1)->context;

		dec->decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
		    dec->manager, r->wayland.toplevel->xdg_top_level);

		zxdg_toplevel_decoration_v1_set_mode(
		    dec->decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
	} else {
		printf("zxdg null\n");
	}

	/*egl_resetup(r);
	MwLLDispatch(r, draw, NULL);
	recursive_draw(r);*/
}

/* Sublevel setup function */
static void setup_sublevel(MwLL parent, MwLL r, int x, int y) {
	struct wl_compositor* compositor     = parent->wayland.compositor;
	struct wl_surface*    parent_surface = parent->wayland.surface;

	r->wayland.sublevel = malloc(sizeof(struct _MwLLWaylandSublevel));

	r->wayland.type = MWLL_WAYLAND_SUBLEVEL;

	r->wayland.display = parent->wayland.display;

	r->wayland.surface = wl_compositor_create_surface(compositor);

	setup_callbacks(&r->wayland);

	// printf("position %d %d\n", x, y);

	r->wayland.registry = wl_display_get_registry(parent->wayland.display);
	r->wayland.display  = parent->wayland.display;

	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}

	r->wayland.sublevel->subsurface = wl_subcompositor_get_subsurface(r->wayland.sublevel->subcompositor, r->wayland.surface, parent_surface);

	wl_subsurface_set_position(r->wayland.sublevel->subsurface, x, y);

	r->wayland.configured = MwTRUE;
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	r = malloc(sizeof(*r));
	MwLLCreateCommon(r);

	r->common.type = MwLLBackendWayland;

	if(width < 2) width = 2;
	if(height < 2) height = 2;

	if(x == MwDEFAULT) {
		x = 0;
	}
	if(y == MwDEFAULT) {
		y = 0;
	}

	r->wayland.ww = width;
	r->wayland.wh = height;
	r->wayland.x  = x;
	r->wayland.y  = y;

	r->wayland.sublevels = NULL;

	if(parent == NULL) {
		setup_toplevel(r, x, y);
	} else {
		setup_sublevel(parent, r, x, y);
	}

	buffer_setup(&r->wayland);

	r->wayland.region = wl_compositor_create_region(r->wayland.compositor);
	region_setup(r);

	MwLLForceRender(r);

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

	buffer_destroy(&handle->wayland);
	wl_region_destroy(handle->wayland.region);

	free(handle);
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	*x = handle->wayland.x;
	*y = handle->wayland.y;
	*w = handle->wayland.ww;
	*h = handle->wayland.wh;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	if(handle->wayland.type != MWLL_WAYLAND_TOPLEVEL) {
		region_invalidate(handle);
		handle->wayland.x = x;
		handle->wayland.y = y;
		wl_subsurface_set_position(handle->wayland.sublevel->subsurface, x, y);
		region_setup(handle);
	}

	MwLLDispatch(handle, draw, NULL);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	region_invalidate(handle);
	/* Prevent an integer underflow when the w/h is too low */
	if((w < 10 || h < 10)) {
		handle->wayland.ww = 10;
		handle->wayland.wh = 10;
		goto refresh;
	}
	handle->wayland.ww = w;
	handle->wayland.wh = h;

	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		xdg_surface_set_window_geometry(handle->wayland.toplevel->xdg_surface, 0, 0, handle->wayland.ww, handle->wayland.wh);
	}

refresh:
	region_setup(handle);

	buffer_destroy(&handle->wayland);
	buffer_setup(&handle->wayland);
	MwLLDispatch(handle, draw, NULL);
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int i;

	cairo_set_source_rgb(handle->wayland.cairo, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0);
	cairo_new_path(handle->wayland.cairo);
	for(i = 0; i < points_count; i++) {
		if(i == 0) {
			cairo_move_to(handle->wayland.cairo, points[i].x, points[i].y);
		} else {
			cairo_line_to(handle->wayland.cairo, points[i].x, points[i].y);
		}
	}
	cairo_close_path(handle->wayland.cairo);
	cairo_fill(handle->wayland.cairo);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	int i;

	cairo_set_line_cap(handle->wayland.cairo, CAIRO_LINE_CAP_SQUARE);
	cairo_set_source_rgb(handle->wayland.cairo, color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0);
	cairo_new_path(handle->wayland.cairo);
	for(i = 0; i < 2; i++) {
		if(i == 0) {
			cairo_move_to(handle->wayland.cairo, points[i].x, points[i].y);
		} else {
			cairo_line_to(handle->wayland.cairo, points[i].x, points[i].y);
		}
	}
	cairo_close_path(handle->wayland.cairo);
	cairo_stroke(handle->wayland.cairo);
}

static void MwLLBeginDrawImpl(MwLL handle) {
}

static void MwLLEndDrawImpl(MwLL handle) {
	update_buffer(handle);
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

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static int MwLLPendingImpl(MwLL handle) {
	return event_loop(handle);
}

static void MwLLNextEventImpl(MwLL handle) {
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		xdg_toplevel_set_title(handle->wayland.toplevel->xdg_top_level, title);
	}
}

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));

	r->common.width	 = width;
	r->common.height = height;
	r->common.raw	 = malloc(4 * width * height);
	memcpy(r->common.raw, data, 4 * width * height);

	r->wayland.cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

	MwLLPixmapUpdate(r);

	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	int	       i;
	unsigned char* d;

	cairo_surface_flush(r->wayland.cs);
	d = cairo_image_surface_get_data(r->wayland.cs);
	for(i = 0; i < r->common.width * r->common.height * 4; i += 4) {
		MwU32* p = (MwU32*)&d[i];
		*p <<= 8;
		*p |= r->common.raw[i + 3];
		*p <<= 8;
		*p |= r->common.raw[i + 0];
		*p <<= 8;
		*p |= r->common.raw[i + 1];
		*p <<= 8;
		*p |= r->common.raw[i + 2];
	}
	cairo_surface_mark_dirty(r->wayland.cs);
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	cairo_surface_destroy(pixmap->wayland.cs);
	free(pixmap->common.raw);
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	cairo_t*	 c;
	cairo_surface_t* cs;

	cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, rect->width, rect->height);
	c  = cairo_create(cs);

	cairo_scale(c, (double)rect->width / pixmap->common.width, (double)rect->height / pixmap->common.height);
	cairo_set_source_surface(c, pixmap->wayland.cs, 0, 0);
	cairo_pattern_set_filter(cairo_get_source(c), CAIRO_FILTER_NEAREST);
	cairo_paint(c);

	cairo_set_source_rgb(handle->wayland.cairo, 1, 1, 1);
	cairo_set_source_surface(handle->wayland.cairo, cs, rect->x, rect->y);
	cairo_paint(handle->wayland.cairo);

	cairo_destroy(c);
	cairo_surface_destroy(cs);
}
static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
}

static void MwLLForceRenderImpl(MwLL handle) {
	wl_surface_damage(handle->wayland.surface, 0, 0, handle->wayland.ww, handle->wayland.wh);

	/*
	if(handle->wayland.egl_setup) {
		timed_redraw_by_epoch(handle, 25);
	}
	*/
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
	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		xdg_toplevel_set_min_size(handle->wayland.toplevel->xdg_top_level, minx, miny);
		xdg_toplevel_set_max_size(handle->wayland.toplevel->xdg_top_level, maxx, maxy);
	}
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		if(WAYLAND_GET_INTERFACE(handle->wayland, zxdg_decoration_manager_v1) != NULL) {
			zxdg_decoration_manager_v1_context_t* dec = WAYLAND_GET_INTERFACE(handle->wayland, zxdg_decoration_manager_v1)->context;

			dec->decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(
			    dec->manager, handle->wayland.toplevel->xdg_top_level);

			zxdg_toplevel_decoration_v1_set_mode(
			    dec->decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE);
		} else {
			/* TODO: hide our custom window decorations when we have them. */
			printf("zxdg null\n");
		}
	}
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

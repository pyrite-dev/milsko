#include <Mw/Milsko.h>

#include <poll.h>
#include <wayland-client-protocol.h>

#include "../../external/stb_ds.h"

#include <sys/mman.h>
#include <wayland-egl-core.h>
#include <wayland-util.h>

/* TODO: find out what FreeBSD and such wants us to include */
#ifdef __FreeBSD__
/* XXX: Untested (nishi) */
#include <evdev/input.h>
#else
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

/*
 * TODO:
 * make sure MwLLDestroy is finished (+ handle dropdown bug that results from it)
 * GNOME doesn't want to support zxdg_decoration_protocol so we're gonna design some Windows 95 ass ui just for that window manager.
 * finish the rest of the owl
 */

/*
 * Redraw `handle` if the given cooldown hasn't expired (`time` >= `cooldown_timer` + `cooldown)).
 * Used for pointer callbacks, resize callbacks, etc. to ensure we only resize every x milliseconds
 */
static void timed_redraw(MwLL handle, MwU32 time, int cooldown, MwU64* cooldown_timer);

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
	p.point			      = self->wayland.cur_mouse_pos;
	MwLLDispatch(self, move, &p);

	timed_redraw(self, time, 50, &self->wayland.cooldown_timer);
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

/* Recursively dispatch the draw event to `handle` and its children */
static void recursive_draw(MwLL handle) {
	int i;
	if(handle->wayland.sublevels != NULL) {
		for(i = 0; i < arrlen(handle->wayland.sublevels); i++) {
			MwLLDispatch(handle->wayland.sublevels[i], draw, NULL);
			recursive_draw(handle->wayland.sublevels[i]);
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
	wayland->subcompositor = wl_registry_bind(wayland->registry, name, &wl_subcompositor_interface, 1);

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

/* EGL Setup function */
static MwBool egl_setup(MwLL self, int x, int y, int width, int height) {
	int	   err;
	EGLint	   numConfigs;
	EGLint	   majorVersion;
	EGLint	   minorVersion;
	EGLContext context;
	EGLSurface surface;
	EGLint	   fbAttribs[] =
	    {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE};
	EGLint contextAttribs[] = {
	    EGL_CONTEXT_CLIENT_VERSION, 1,
	    EGL_CONTEXT_MAJOR_VERSION, 1,
	    EGL_CONTEXT_MINOR_VERSION, 1,
	    EGL_NONE};

	EGLDisplay display;
	display = eglGetDisplay(self->wayland.display);
	if(display == EGL_NO_DISPLAY) {
		printf("ERROR: eglGetDisplay, %0X\n", eglGetError());
		return MwFALSE;
	}
	/* Initialize EGL */
	if(!eglInitialize(display, &majorVersion, &minorVersion)) {
		printf("ERROR: eglInitialize, %0X\n", eglGetError());
		return MwFALSE;
	}

	/* Get configs */
	if((eglGetConfigs(display, NULL, 0, &numConfigs) != EGL_TRUE) || (numConfigs == 0)) {
		printf("ERROR: eglGetConfigs, %0X\n", eglGetError());
		return MwFALSE;
	}

	/* Choose config */
	if((eglChooseConfig(display, fbAttribs, &self->wayland.egl_config, 1, &numConfigs) != EGL_TRUE) || (numConfigs != 1)) {
		printf("ERROR: eglChooseConfig, %0X\n", eglGetError());
		return MwFALSE;
	}

	self->wayland.egl_window_native =
	    wl_egl_window_create(self->wayland.surface, width, height);
	if(self->wayland.egl_window_native == EGL_NO_SURFACE) {
		printf("ERROR: wl_egl_window_create, EGL_NO_SURFACE\n");
		return MwFALSE;
	}

	/* Create a surface */
	surface = eglCreateWindowSurface(display, self->wayland.egl_config, self->wayland.egl_window_native, NULL);
	if(surface == EGL_NO_SURFACE) {
		printf("ERROR: eglCreateWindowSurface, %0X\n", eglGetError());
		return MwFALSE;
	}

	eglBindAPI(EGL_OPENGL_API);

	/* Create a GL context */
	context = eglCreateContext(display, self->wayland.egl_config, EGL_NO_CONTEXT, contextAttribs);
	if(context == EGL_NO_CONTEXT) {
		printf("ERROR: eglCreateContext, %0X\n", eglGetError());
		return MwFALSE;
	}

	self->wayland.egl_display = display;
	self->wayland.egl_surface = surface;
	self->wayland.egl_context = context;

	if(self->wayland.parent == NULL) {
		if(!eglMakeCurrent(self->wayland.egl_display, self->wayland.egl_surface, self->wayland.egl_surface, self->wayland.egl_context)) {
			printf("ERROR: eglMakeCurrent, %0X\n", eglGetError());
		}
	}

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);

	return MwTRUE;
}

/* EGL reconfiguration function for resizes */
static void egl_resetup(MwLL handle) {
	float w, h;

	if(handle->wayland.parent == NULL) {
		w = handle->wayland.ww;
		h = handle->wayland.wh;
	} else {
		w = handle->wayland.parent->wayland.ww;
		h = handle->wayland.parent->wayland.wh;
	}

	glViewport(handle->wayland.x, handle->wayland.y, w, h);
	glScissor(handle->wayland.x, handle->wayland.y, w, h);
}

static void timed_redraw(MwLL handle, MwU32 time, int cooldown, MwU64* cooldown_timer) {
	if(handle->wayland.parent == NULL) {
		if(time >= (*cooldown_timer + cooldown)) {
			egl_resetup(handle);
			MwLLDispatch(handle, draw, NULL);
			*cooldown_timer = time;
		}
	}
}

/* Same as timed_redraw but it uses the unix epoch as the cooldown timer. */
static void timed_redraw_by_epoch(MwLL handle, int cooldown) {
	gettimeofday(&handle->wayland.timer, NULL);
	unsigned long long time =
	    (unsigned long long)(handle->wayland.timer.tv_sec) * 1000 +
	    (unsigned long long)(handle->wayland.timer.tv_usec) / 1000;
	timed_redraw(handle, time, cooldown, &handle->wayland.cooldown_timer_epoch);
}

/* `xdg_toplevel.close` callback */
static void xdg_toplevel_configure(void*		data,
				   struct xdg_toplevel* xdg_toplevel,
				   MwI32 width, MwI32 height,
				   struct wl_array* states) {
	MwLL self = data;

	if(width == 0 || height == 0) {
		width  = self->wayland.ww;
		height = self->wayland.wh;
		/* if it's still 0 then bail */
		if(width == 0 || height == 0) {
			return;
		}
	}

	self->wayland.ww = width;
	self->wayland.wh = height;
	xdg_surface_set_window_geometry(self->wayland.toplevel->xdg_surface, 0, 0, self->wayland.ww, self->wayland.wh);

	MwLLDispatch(self, resize, NULL);

	if(!self->wayland.egl_setup) {
		self->wayland.egl_setup = egl_setup(self, self->wayland.x, self->wayland.y, width, height);
	} else {
		wl_egl_window_resize(self->wayland.egl_window_native, width, height, 0, 0);
		egl_resetup(self);
	}

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

/* Standard Wayland event loop. */
static int event_loop(MwLL handle) {
	enum {
		DISPLAY_FD,
		KEYREPEAT_FD,
		CURSOR_FD
	};
	struct pollfd	     fd;
	int		     timeout	    = 100;
	struct _MwLLWayland* wayland	    = &handle->wayland;
	MwLL		     topmost_parent = handle->wayland.parent;

	if(wayland->display == NULL) {
		return 0;
	}
	fd.fd	  = wl_display_get_fd(wayland->display);
	fd.events = POLLIN;

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

	if(fd.revents & POLLIN) {
		wl_display_read_events(wayland->display);
		if(wl_display_dispatch_pending(wayland->display) > 0) {
		}
	} else {
		/* Condition for no events being sent */
		if(wl_display_dispatch_pending(wayland->display) == 0) {
			if(wayland->event_queue != NULL) {
				if(wl_display_roundtrip_queue(wayland->display, wayland->event_queue) < 0) {
					printf("error\n");
				};
			}
			/* HACK: If the last known size is different, force a redraw */
			if(wayland->lw != wayland->ww || wayland->lh != wayland->wh) {
				MwLLDispatch(handle, draw, 0);
				wayland->resize_counter++;
				if(wayland->resize_counter >= 5) {
					wayland->lw		= wayland->ww;
					wayland->lh		= wayland->wh;
					wayland->lh		= wayland->wh;
					wayland->resize_counter = 0;
				}
			}
		}
	}

	return 0;
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

	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(wl_subcompositor);
	WL_INTERFACE(wl_seat);
	if(wayland->type == MWLL_WAYLAND_TOPLEVEL) {
		WL_INTERFACE(xdg_wm_base);
		WL_INTERFACE(zxdg_decoration_manager_v1);
		WL_INTERFACE(wp_cursor_shape_manager_v1);
	}
#undef WL_INTERFACE
}

/* Toplevel setup function */
static void setup_toplevel(MwLL r, int x, int y) {
	int i;

	r->wayland.type		  = MWLL_WAYLAND_TOPLEVEL;
	r->wayland.toplevel	  = malloc(sizeof(struct _MwLLWaylandTopLevel));
	r->wayland.topmost_parent = NULL;

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

	r->wayland.event_queue = wl_display_create_queue(r->wayland.display);

	egl_resetup(r);
	MwLLDispatch(r, draw, NULL);
	recursive_draw(r);
}

/* Sublevel setup function */
static void setup_sublevel(MwLL parent, MwLL r, int x, int y) {
	struct wl_compositor*	 compositor	= parent->wayland.compositor;
	struct wl_subcompositor* subcompositor	= parent->wayland.subcompositor;
	struct wl_surface*	 parent_surface = parent->wayland.surface;
	struct wl_region*	 region;
	{
		MwLL topmost_parent = parent;

		while(topmost_parent->wayland.type != MWLL_WAYLAND_TOPLEVEL) {
			topmost_parent = topmost_parent->wayland.parent;
		}
		r->wayland.topmost_parent = topmost_parent;
	}

	r->wayland.type = MWLL_WAYLAND_SUBLEVEL;

	r->wayland.display = parent->wayland.display;

	r->wayland.surface = wl_compositor_create_surface(compositor);

	region = wl_compositor_create_region(compositor);
	wl_region_add(region, 0, 0, r->wayland.ww, r->wayland.wh);
	wl_surface_set_opaque_region(r->wayland.surface, region);
	wl_surface_commit(r->wayland.surface);
	event_loop(r);
	wl_region_destroy(region);

	setup_callbacks(&r->wayland);

	r->wayland.registry = wl_display_get_registry(r->wayland.topmost_parent->wayland.display);
	r->wayland.display  = r->wayland.topmost_parent->wayland.display;

	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}

	r->wayland.configured = MwTRUE;
	if(!r->wayland.egl_setup) {
		r->wayland.egl_setup = egl_setup(r, r->wayland.x, r->wayland.y, r->wayland.ww, r->wayland.wh);
		egl_resetup(r);
		MwLLDispatch(r, draw, NULL);
		recursive_draw(r);
	};

	r->wayland.event_queue = parent->wayland.event_queue;

	arrpush(parent->wayland.sublevels, r);

	wl_surface_damage(parent->wayland.surface, 0, 0, parent->wayland.ww, parent->wayland.wh);

	egl_resetup(r->wayland.topmost_parent);
	MwLLDispatch(r->wayland.topmost_parent, draw, NULL);
	recursive_draw(r->wayland.topmost_parent);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	r = malloc(sizeof(*r));
	MwLLCreateCommon(r);

	r->common.type = MwLLBackendWayland;

	if(width == 0) width = 1;
	if(height == 0) height = 1;

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

	r->wayland.parent = parent;

	r->wayland.cooldown_timer = 0;
	r->wayland.has_set_xy	  = MwFALSE;
	r->wayland.resize_counter = 0;

	r->wayland.sublevels = NULL;

	if(parent == NULL) {
		setup_toplevel(r, x, y);
	} else {
		setup_sublevel(parent, r, x, y);
	}

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	MwLLDestroyCommon(handle);

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
		handle->wayland.x = x;
		handle->wayland.y = y;
		if(handle->wayland.has_set_xy) {
			timed_redraw_by_epoch(handle->wayland.topmost_parent, 25);
			/* recursive_draw(handle->wayland.topmost_parent); */
		} else if(x != 0 && y != 0) {
			handle->wayland.has_set_xy = MwTRUE;
		}
	}
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	/* Prevent an integer underflow when the w/h is too low */
	if((w < 10 || h < 10)) {
		handle->wayland.ww = handle->wayland.lw = 10;
		handle->wayland.wh = handle->wayland.lh = 10;
		return;
	}
	handle->wayland.ww = handle->wayland.lw = w;
	handle->wayland.wh = handle->wayland.lh = h;

	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		xdg_surface_set_window_geometry(handle->wayland.toplevel->xdg_surface, 0, 0, handle->wayland.ww, handle->wayland.wh);
	} else {
		timed_redraw_by_epoch(handle->wayland.topmost_parent, 25);
		/* recursive_draw(handle->wayland.topmost_parent); */
	}
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	int   i, n;
	float maxX = 0, maxY = 0;
	float centerX, centerY;
	float w, h;

	if(handle->wayland.parent == NULL) {
		w = handle->wayland.ww;
		h = handle->wayland.wh;
	} else {
		w = handle->wayland.topmost_parent->wayland.ww;
		h = handle->wayland.topmost_parent->wayland.wh;
	}

	glColor3f(color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0);

	glBegin(GL_POLYGON);

	for(i = 0; i < points_count; i++) {
		float x = ((float)(handle->wayland.x + points[i].x) / (w / 2)) - 1.0;
		float y = 1.0 - ((float)(handle->wayland.y + points[i].y) / (h / 2));

		glVertex2f(x, y);
	}
	glEnd();
	glColor3f(0, 0, 0);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	int   i;
	float w, h;

	if(handle->wayland.topmost_parent == NULL) {
		w = handle->wayland.ww;
		h = handle->wayland.wh;
	} else {
		w = handle->wayland.topmost_parent->wayland.ww;
		h = handle->wayland.topmost_parent->wayland.wh;
	}

	glLineWidth(1);
	glColor3f(color->common.red / 255.0, color->common.green / 255.0, color->common.blue / 255.0);

	glBegin(GL_LINES);
	for(i = 0; i < 2; i++) {
		float x = ((float)(handle->wayland.x + points[i].x) / (w / 2)) - 1.0;
		float y = 1.0 - ((float)(handle->wayland.y + points[i].y) / (h / 2));

		glVertex2f(x, y);
	}
	glEnd();
	glColor3f(0, 0, 0);
}

static void MwLLBeginDrawImpl(MwLL handle) {
	if(handle->wayland.parent == NULL) {
		if(!eglMakeCurrent(handle->wayland.egl_display, handle->wayland.egl_surface, handle->wayland.egl_surface, handle->wayland.egl_context)) {
			printf("ERROR: eglMakeCurrent, %0X\n", eglGetError());
			return;
		}
	}
}

static void MwLLEndDrawImpl(MwLL handle) {
	int w, h;

	recursive_draw(handle);

	/* glClear(GL_COLOR_BUFFER_BIT); */
	/* glClearColor(1.0, 0, 0, 1); */

	if(handle->wayland.parent == NULL) {
		if(!eglSwapBuffers(handle->wayland.egl_display, handle->wayland.egl_surface)) {
			printf("error swapping buffers! %0X\n", eglGetError());
		} else {
			wl_surface_commit(handle->wayland.surface);
		}
	}
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
	if(wl_display_dispatch_pending(handle->wayland.display) == 0) {
	}
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

	r->common.width	   = width;
	r->common.height   = height;
	r->common.raw	   = data;
	r->wayland.texture = 0;

	glGenTextures(1, &r->wayland.texture);

	glBindTexture(GL_TEXTURE_2D, r->wayland.texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	MwLLPixmapUpdate(r);

	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	glBindTexture(GL_TEXTURE_2D, r->wayland.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r->common.width, r->common.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, r->common.raw);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	glDeleteTextures(1, &pixmap->wayland.texture);
	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	float x, y, w, h;

	if(handle->wayland.topmost_parent == NULL) {
		w = handle->wayland.ww;
		h = handle->wayland.wh;
	} else {
		w = handle->wayland.topmost_parent->wayland.ww;
		h = handle->wayland.topmost_parent->wayland.wh;
	}

	x = ((float)(handle->wayland.x + rect->x) / (w / 2)) - 1.0;
	y = 1.0 - ((float)(handle->wayland.y + rect->y) / (h / 2));

	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, pixmap->wayland.texture);
	glActiveTexture(pixmap->wayland.texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0);
	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + (rect->width / (w / 2)), y);
	glTexCoord2f(1, 1);
	glVertex2f(x + (rect->width / (w / 2)), y - (rect->height / (h / 2)));
	glTexCoord2f(0.0f, 1);
	glVertex2f(x, y - (rect->height / (h / 2)));
	glEnd();
	glFinish();
}
static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
}

static void MwLLForceRenderImpl(MwLL handle) {
	wl_surface_damage(handle->wayland.surface, 0, 0, handle->wayland.ww, handle->wayland.wh);

	if(handle->wayland.egl_setup) {
		timed_redraw_by_epoch(handle, 25);
	}
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

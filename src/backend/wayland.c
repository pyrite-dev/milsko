#include <Mw/Milsko.h>

#include <poll.h>
#include <wayland-client-protocol.h>

#include "../../external/stb_ds.h"

#include <sys/mman.h>
#include <wayland-util.h>
#include <unistd.h>

/* TODO:
 * - MwLLGrabPointerImpl
 * - MwLLMakePopupImpl
 * - MwLLShowImpl
 */

/* TODO: find out what FreeBSD and such wants us to include */
#ifdef __FreeBSD__
/* XXX: Untested (nishi) */
#include <evdev/input.h>
#else
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

/* Setup the framebuffer with the saved width/height */
static void framebuffer_setup(struct _MwLLWayland* wayland);
/* Destroy the framebuffer */
static void framebuffer_destroy(struct _MwLLWayland* handle);

static void buffer_destroy(struct _MwLLWaylandShmBuffer* buffer);

static void region_setup(MwLL handle);
static void region_invalidate(MwLL handle);

/* Get the registered interface from r, or NULL if it doesn't currently have it. */
#define WAYLAND_GET_INTERFACE(handle, inter) shget(handle.wl_protocol_map, inter##_interface.name)

/* `wl_registry.global` callback */
static void new_protocol(void* data, struct wl_registry* registry,
			 MwU32 name, const char* interface,
			 MwU32 version) {
	struct _MwLLWayland* wayland = data;

	wayland_protocol_callback_table_t* cb = shget(wayland->wl_protocol_setup_map, interface);
	if(cb != NULL) {
		char* inter = malloc(strlen(interface) + 1);
		strcpy(inter, interface);
		shput(wayland->wl_protocol_map, inter, cb->setup(name, data));
	} else {
		/* printf("unknown interface %s\n", interface); */
	}
};

/* `wl_registry.global_remove` callback */
static void protocol_removed(void*		 data,
			     struct wl_registry* registry,
			     MwU32		 name) {
	printf("%d destroyed\n", name);
};

/* Flush Wayland events */
static void wl_flush(MwLL handle) {
	struct pollfd pfd;
	int	      rc;

	pfd.fd	   = wl_display_get_fd(handle->wayland.display);
	pfd.events = POLLIN;
	while(MwTRUE) {
		rc = wl_display_flush(handle->wayland.display);
		if(errno != EAGAIN || rc != -1) {
			break;
		}

		if(poll(&pfd, 1, -1) == -1) {
			break;
		}
	}
}

static void wl_offer(void*		   data,
		     struct wl_data_offer* wl_data_offer,
		     const char*	   mime_type) {
	wl_clipboard_device_context_t* self = data;
	wl_data_offer_accept(wl_data_offer, self->ll->wayland.clipboard_serial, mime_type);
};

struct wl_data_offer_listener offer_listener = {
    .offer = wl_offer,
};

static void wl_data_device_data_offer(void*		     data,
				      struct wl_data_device* wl_data_device,
				      struct wl_data_offer*  offer) {
	wl_clipboard_device_context_t* self = data;

	wl_data_offer_add_listener(offer, &offer_listener, data);

	self->offer.wl = offer;
};

static void wl_data_device_enter(void*			data,
				 struct wl_data_device* wl_data_device,
				 uint32_t		serial,
				 struct wl_surface*	surface,
				 wl_fixed_t		x,
				 wl_fixed_t		y,
				 struct wl_data_offer*	id) {
	MwLL self		       = data;
	self->wayland.clipboard_serial = serial;
};
static void wl_data_device_leave(void*			data,
				 struct wl_data_device* wl_data_device) {
	wl_data_device_destroy(wl_data_device);
};
static void wl_data_device_motion(void*			 data,
				  struct wl_data_device* wl_data_device,
				  uint32_t		 time,
				  wl_fixed_t		 x,
				  wl_fixed_t		 y) {
};
static void wl_data_device_drop(void*		       data,
				struct wl_data_device* wl_data_device) {
};

static void wl_data_device_selection(void*		    data,
				     struct wl_data_device* wl_data_device,
				     struct wl_data_offer*  id) {
};

struct wl_data_device_listener wl_data_device_listener = {
    .data_offer = wl_data_device_data_offer,
    .enter	= wl_data_device_enter,
    .leave	= wl_data_device_leave,
    .motion	= wl_data_device_motion,
    .drop	= wl_data_device_drop,
    .selection	= wl_data_device_selection,
};

static void zwp_primary_selection_device_v1_data_offer(void*				       data,
						       struct zwp_primary_selection_device_v1* zwp_primary_selection_device_v1,
						       struct zwp_primary_selection_offer_v1*  offer) {
	wl_clipboard_device_context_t* self = data;

	self->offer.zwp = offer;
};

static void zwp_primary_selection_device_v1_selection(void*				      data,
						      struct zwp_primary_selection_device_v1* zwp_primary_selection_device_v1,
						      struct zwp_primary_selection_offer_v1*  id) {
};

struct zwp_primary_selection_device_v1_listener zwp_primary_selection_device_v1_listener = {
    .data_offer = zwp_primary_selection_device_v1_data_offer,
    .selection	= zwp_primary_selection_device_v1_selection,
};

static void wl_clipboard_read(wl_clipboard_device_context_t* ctx) {
	int	       fds[2];
	fd_set	       set;
	char*	       buf = NULL;
	struct timeval timeout;
	int	       rc = 0;
	int	       n;

	if(pipe(fds) != 0) {
		return;
	}

	FD_ZERO(&set);
	FD_SET(fds[0], &set);
	timeout.tv_sec	= 0;
	timeout.tv_usec = 100;

	if(ctx->ll->wayland.supports_zwp) {
		zwp_primary_selection_offer_v1_receive(ctx->offer.zwp, "text/plain", fds[1]);
	} else {
		wl_data_offer_receive(ctx->offer.wl, "text/plain", fds[1]);
	}
	close(fds[1]);

	wl_flush(ctx->ll);
	wl_display_roundtrip(ctx->ll->wayland.display);

	while(MwTRUE) {
		char ch;
		rc = select(fds[0] + 1, &set, NULL, NULL, &timeout);
		if(rc <= 0) {
			break;
		} else {
			char	b;
			ssize_t n = read(fds[0], &b, sizeof(b));
			if(n <= 0) {
				break;
			}
			arrpush(buf, b);
		}
	}
	arrpush(buf, 0);
	close(fds[0]);

	MwLLDispatch(ctx->ll, clipboard, buf);
	arrfree(buf);
}

static void wl_data_source_listener_target(void*		  data,
					   struct wl_data_source* wl_data_source,
					   const char*		  mime_type) {
};
static void wl_data_source_listener_send(void*			data,
					 struct wl_data_source* wl_data_source,
					 const char*		mime_type,
					 int32_t		fd) {
	MwLL self = data;
	if(self->wayland.clipboard_buffer != NULL) {
		write(fd, self->wayland.clipboard_buffer, strlen(self->wayland.clipboard_buffer));
		close(fd);

		self->wayland.events_pending = 1;
	}
};
static void wl_data_source_listener_cancelled(void*		     data,
					      struct wl_data_source* wl_data_source);

struct wl_data_source_listener wl_data_source_listener = {
    .target    = wl_data_source_listener_target,
    .send      = wl_data_source_listener_send,
    .cancelled = wl_data_source_listener_cancelled,
};

static void wl_data_source_listener_cancelled(void*		     data,
					      struct wl_data_source* wl_data_source) {
	MwLL self = data;

	wl_data_source_destroy(wl_data_source);

	self->wayland.clipboard_source.wl = wl_data_device_manager_create_data_source(self->wayland.clipboard_manager.wl);

	wl_data_source_offer(self->wayland.clipboard_source.wl, "text/plain");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "text/plain;charset=utf-8");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "TEXT");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "STRING");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "UTF8_STRING");

	wl_data_source_add_listener(self->wayland.clipboard_source.wl, &wl_data_source_listener, self);
};

static void zwp_primary_selection_source_v1_send(void*					 data,
						 struct zwp_primary_selection_source_v1* wl_data_source,
						 const char*				 mime_type,
						 int32_t				 fd) {
	MwLL self = data;
	if(self->wayland.clipboard_buffer != NULL) {
		write(fd, self->wayland.clipboard_buffer, strlen(self->wayland.clipboard_buffer));
		close(fd);

		self->wayland.events_pending = 1;
	}
};
static void zwp_primary_selection_source_v1_cancelled(void*				      data,
						      struct zwp_primary_selection_source_v1* wl_data_source) {
	MwLL self = data;

	zwp_primary_selection_source_v1_destroy(self->wayland.clipboard_source.zwp);

	self->wayland.clipboard_source.zwp = zwp_primary_selection_device_manager_v1_create_source(self->wayland.clipboard_manager.zwp);

	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "text/plain");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "text/plain;charset=utf-8");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "TEXT");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "STRING");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "UTF8_STRING");
};

struct zwp_primary_selection_source_v1_listener zwp_primary_selection_source_v1_listener = {
    .send      = zwp_primary_selection_source_v1_send,
    .cancelled = zwp_primary_selection_source_v1_cancelled,
};

/* wl_data_device_manager setup function */
static wayland_protocol_t* wl_data_device_manager_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->clipboard_manager.wl = wl_registry_bind(wayland->registry, name, &wl_data_device_manager_interface, 2);

	wayland->clipboard_source.wl = wl_data_device_manager_create_data_source(wayland->clipboard_manager.wl);

	wl_data_source_offer(wayland->clipboard_source.wl, "text/plain");
	wl_data_source_offer(wayland->clipboard_source.wl, "text/plain;charset=utf-8");
	wl_data_source_offer(wayland->clipboard_source.wl, "TEXT");
	wl_data_source_offer(wayland->clipboard_source.wl, "STRING");
	wl_data_source_offer(wayland->clipboard_source.wl, "UTF8_STRING");

	wl_data_source_add_listener(wayland->clipboard_source.wl, &wl_data_source_listener, wayland);

	return NULL;
}

static void wl_data_device_manager_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	if(wayland->clipboard_manager.wl != NULL && !wayland->supports_zwp) {
		wl_data_device_manager_destroy(wayland->clipboard_manager.wl);
		wl_data_source_destroy(wayland->clipboard_source.wl);
		wayland->clipboard_manager.wl = NULL;
	}
}

/* zwp_primary_selection_device_manager_v1 setup function */
static wayland_protocol_t* zwp_primary_selection_device_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	if(wayland->clipboard_manager.wl != NULL) {
		wl_data_device_manager_interface_destroy(wayland, NULL);
	}

	wayland->clipboard_manager.zwp = wl_registry_bind(wayland->registry, name, &zwp_primary_selection_device_manager_v1_interface, 1);

	wayland->clipboard_source.zwp = zwp_primary_selection_device_manager_v1_create_source(wayland->clipboard_manager.zwp);

	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "text/plain");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "text/plain;charset=utf-8");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "TEXT");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "STRING");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "UTF8_STRING");

	zwp_primary_selection_source_v1_add_listener(wayland->clipboard_source.zwp, &zwp_primary_selection_source_v1_listener, wayland);

	wayland->supports_zwp = MwTRUE;

	return NULL;
}

static void zwp_primary_selection_device_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	return;

	zwp_primary_selection_device_manager_v1_destroy(wayland->clipboard_manager.zwp);

	zwp_primary_selection_source_v1_destroy(wayland->clipboard_source.zwp);

	free(data->listener);
}

/* `wl_pointer.enter` callback */
static void pointer_enter(void* data, struct wl_pointer* wl_pointer, MwU32 serial,
			  struct wl_surface* surface, wl_fixed_t surface_x,
			  wl_fixed_t surface_y) {
	MwLL		self = data;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};
	self->wayland.pointer_serial = serial;

	wl_pointer_set_cursor(wl_pointer, serial, self->wayland.cursor.surface, 0, 0);

	pthread_mutex_unlock(&self->wayland.eventsMutex);
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

	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};
	self->wayland.cur_mouse_pos.x = wl_fixed_to_double(surface_x);
	self->wayland.cur_mouse_pos.y = wl_fixed_to_double(surface_y);

	p.point = self->wayland.cur_mouse_pos;
	MwLLDispatch(self, move, &p);

	self->wayland.events_pending += 1;
	pthread_mutex_unlock(&self->wayland.eventsMutex);

	/*timed_redraw(self, time, 50, &self->wayland.cooldown_timer);*/
};

/* `wl_pointer.button` callback */
static void pointer_button(void* data, struct wl_pointer* wl_pointer, MwU32 serial, MwU32 time, MwU32 button, MwU32 state) {
	MwLL		self = data;
	MwLLMouse	p;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};

	p.point = self->wayland.cur_mouse_pos;
	if(p.point.x > self->wayland.x && p.point.x < self->wayland.x + self->wayland.ww && p.point.y > self->wayland.y && p.point.y < self->wayland.y + self->wayland.wh) {
		switch(button) {
		case BTN_LEFT:
			p.button = MwLLMouseLeft;
			break;
		case BTN_MIDDLE:
			p.button = MwLLMouseMiddle;
			for(int i = 0; i < arrlen(self->wayland.clipboard_devices); i++) {
				wl_clipboard_read(
				    self->wayland.clipboard_devices[i]);
			}
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

	if(!self->wayland.always_render) {
		MwLLDispatch(self, draw, NULL);
		self->wayland.events_pending += 1;
	}
	pthread_mutex_unlock(&self->wayland.eventsMutex);
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
	MwLL		self = data;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};

	self->wayland.keyboard_serial = serial;

	MwLLDispatch(self, focus_in, NULL);
	self->wayland.events_pending += 1;
	pthread_mutex_unlock(&self->wayland.eventsMutex);
};

/* `wl_keyboard.leave` callback */
static void keyboard_leave(void*	       data,
			   struct wl_keyboard* wl_keyboard,
			   MwU32	       serial,
			   struct wl_surface*  surface) {
	MwLL		self = data;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};

	MwLLDispatch(self, focus_out, NULL);
	self->wayland.events_pending += 1;

	pthread_mutex_unlock(&self->wayland.eventsMutex);
};

/* `wl_keyboard.key` callback */
static void keyboard_key(void*		     data,
			 struct wl_keyboard* wl_keyboard,
			 MwU32		     serial,
			 MwU32		     time,
			 MwU32		     key,
			 MwU32		     state) {
	MwLL		self = data;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};
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
			if((self->wayland.mod_state & 4) == 4) {
				/* clipboard paste */
				if(key == 'V') {
					for(int i = 0; i < arrlen(self->wayland.clipboard_devices); i++) {
						wl_clipboard_read(
						    self->wayland.clipboard_devices[i]);
					}
				}
				key |= MwLLControlMask;
			}
			if((self->wayland.mod_state & 8) == 8) {
				key |= MwLLAltMask;
			}

			if(state == WL_KEYBOARD_KEY_STATE_PRESSED) {
				MwLLDispatch(self, key, &key);
			} else {
				MwLLDispatch(self, key_released, &key);
			}
		}
	}

	if(!self->wayland.always_render) {
		MwLLDispatch(self, draw, NULL);
		self->wayland.events_pending += 1;
	}

	pthread_mutex_unlock(&self->wayland.eventsMutex);
};

/* `wl_keyboard.modifiers` callback */
static void keyboard_modifiers(void*		   data,
			       struct wl_keyboard* wl_keyboard,
			       MwU32		   serial,
			       MwU32		   mods_depressed,
			       MwU32		   mods_latched,
			       MwU32		   mods_locked,
			       MwU32		   group) {
	MwLL		self = data;
	struct timespec t;
	t.tv_nsec = 100;

	if(pthread_mutex_timedlock(&self->wayland.eventsMutex, &t) != 0) {
		return;
	};

	self->wayland.mod_state = 0;
	self->wayland.mod_state |= mods_depressed;
	self->wayland.mod_state |= mods_locked;

	pthread_mutex_unlock(&self->wayland.eventsMutex);
};

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
	MwLL			       self	  = data;
	wl_clipboard_device_context_t* device_ctx = malloc(sizeof(wl_clipboard_device_context_t));

	if(capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
		self->wayland.keyboard = wl_seat_get_keyboard(wl_seat);
		wl_keyboard_add_listener(self->wayland.keyboard, &keyboard_listener, data);
	}
	if(capabilities & WL_SEAT_CAPABILITY_POINTER) {
		self->wayland.pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(self->wayland.pointer, &pointer_listener, data);
	}

	if(self->wayland.clipboard_manager.wl != NULL) {
		if(self->wayland.supports_zwp) {
			device_ctx->device.zwp = zwp_primary_selection_device_manager_v1_get_device(self->wayland.clipboard_manager.zwp, wl_seat);
		} else {
			device_ctx->device.wl = wl_data_device_manager_get_data_device(self->wayland.clipboard_manager.wl, wl_seat);
		}
		device_ctx->ll		 = self;
		device_ctx->seat	 = wl_seat;
		device_ctx->capabilities = capabilities;

		if(self->wayland.supports_zwp) {
			zwp_primary_selection_device_v1_add_listener(device_ctx->device.zwp, &zwp_primary_selection_device_v1_listener, device_ctx);
		} else {
			wl_data_device_add_listener(device_ctx->device.wl, &wl_data_device_listener, device_ctx);
		}

		arrpush(self->wayland.clipboard_devices, device_ctx);
	}
};

static void output_geometry(void*	      data,
			    struct wl_output* wl_output,
			    int32_t	      x,
			    int32_t	      y,
			    int32_t	      physical_width,
			    int32_t	      physical_height,
			    int32_t	      subpixel,
			    const char*	      make,
			    const char*	      model,
			    int32_t	      transform) {};
static void output_mode(void*		  data,
			struct wl_output* wl_output,
			uint32_t	  flags,
			int32_t		  width,
			int32_t		  height,
			int32_t		  refresh) {
	MwLL self	 = data;
	self->wayland.mw = width;
	self->wayland.mh = height;
};

struct wl_output_listener output_listener = {
    .geometry = output_geometry,
    .mode     = output_mode,
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

static void wl_seat_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	free(data->listener);
	wl_seat_destroy(data->context);
}

/* wl_output setup function */
static wayland_protocol_t* wl_output_setup(MwU32 name, MwLL ll) {
	ll->wayland.output = wl_registry_bind(ll->wayland.registry, name, &wl_output_interface, 1);
	wl_output_add_listener(ll->wayland.output, &output_listener, ll);

	return NULL;
}

static void wl_output_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
}

/* wl_compositor setup function */
static wayland_protocol_t* wl_compositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->compositor = wl_registry_bind(wayland->registry, name, &wl_compositor_interface, 1);

	return NULL;
}

static void wl_compositor_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
}

/* wl_subcompositor setup function */
static wayland_protocol_t* wl_subcompositor_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland->sublevel->subcompositor = wl_registry_bind(wayland->registry, name, &wl_subcompositor_interface, 1);

	return NULL;
}

static void wl_subcompositor_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	wl_subcompositor_destroy(wayland->sublevel->subcompositor);
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

static void xdg_wm_base_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	xdg_wm_base_destroy(data->context);
	free(data->listener);
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

static void zxdg_decoration_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	zxdg_decoration_manager_v1_context_t* context = data->context;

	zxdg_decoration_manager_v1_destroy(context->manager);
	zxdg_toplevel_decoration_v1_destroy(context->decoration);
}

/* `xdg_toplevel.close` callback */
static void xdg_toplevel_configure(void*		data,
				   struct xdg_toplevel* xdg_toplevel,
				   MwI32 width, MwI32 height,
				   struct wl_array* states) {
	MwLL self = data;
	int  i;

	/* Yes, somehow this can get called before  */
	if(!self->wayland.configured) {
		return;
	}

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

	framebuffer_destroy(&self->wayland);
	framebuffer_setup(&self->wayland);
	region_setup(self);

	MwLLDispatch(self, resize, NULL);
	MwLLDispatch(self, draw, NULL);

	MwLLForceRender(self);

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
		wl_surface_commit(self->wayland.framebuffer.surface);
	}

	self->wayland.configured = MwTRUE;
}

/* wl_shm setup function */
static wayland_protocol_t* wl_shm_setup(MwU32 name, struct _MwLLWayland* wayland) {

	wayland->framebuffer.shm = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);
	wayland->cursor.shm	 = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);

	return NULL;
}

static void wl_shm_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
}

static void update_buffer(struct _MwLLWaylandShmBuffer* buffer) {
	fsync(buffer->fd);

	wl_surface_attach(buffer->surface, buffer->shm_buffer, 0, 0);
	wl_surface_commit(buffer->surface);
}

static void buffer_setup(struct _MwLLWaylandShmBuffer* buffer, MwU32 width, MwU32 height) {
	int  x, y;
	int  stride	 = width * 4;
	char temp_name[] = "/tmp/milsko-wl-shm-XXXXXX";

	buffer->buf_size = width * height * 4;

	buffer->fd = mkstemp(temp_name);

	unlink(temp_name);

	if(posix_fallocate(buffer->fd, 0, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not fallocate. %s.\n", strerror(errno));
		close(buffer->fd);
		return;
	}
	if(ftruncate(buffer->fd, buffer->buf_size) != 0) {
		printf("failure setting up wl_shm: could not truncate. %s.\n", strerror(errno));
		close(buffer->fd);
		return;
	}

	buffer->buf = mmap(NULL, buffer->buf_size, PROT_WRITE, MAP_SHARED, buffer->fd, 0);

	fsync(buffer->fd);

	if(!(buffer->shm_pool = wl_shm_create_pool(buffer->shm, buffer->fd, buffer->buf_size))) {
		printf("failure setting up wl_shm: could not create pool.\n");
	}

	buffer->shm_buffer = wl_shm_pool_create_buffer(buffer->shm_pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
	buffer->setup	   = MwTRUE;
}

static void buffer_destroy(struct _MwLLWaylandShmBuffer* buffer) {
	if(!buffer->setup) {
		return;
	}
	wl_buffer_destroy(buffer->shm_buffer);
	wl_shm_pool_destroy(buffer->shm_pool);
	close(buffer->fd);
	buffer->setup = MwFALSE;
}

static void framebuffer_setup(struct _MwLLWayland* wayland) {
	buffer_setup(&wayland->framebuffer, wayland->ww, wayland->wh);

	wayland->cs    = cairo_image_surface_create_for_data(wayland->framebuffer.buf, CAIRO_FORMAT_ARGB32, wayland->ww, wayland->wh, 4 * wayland->ww);
	wayland->cairo = cairo_create(wayland->cs);

	memset(wayland->framebuffer.buf, 255, wayland->framebuffer.buf_size);
	update_buffer(&wayland->framebuffer);

	wayland->events_pending += 1;
};
static void framebuffer_destroy(struct _MwLLWayland* wayland) {
	buffer_destroy(&wayland->framebuffer);
	cairo_destroy(wayland->cairo);
	cairo_surface_destroy(wayland->cs);
};

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
	wl_surface_set_input_region(handle->wayland.framebuffer.surface, handle->wayland.region);
	wl_surface_set_opaque_region(handle->wayland.framebuffer.surface, handle->wayland.region);
}

static wayland_protocol_t* xdg_toplevel_icon_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));

	proto->context	= wl_registry_bind(wayland->registry, name, &xdg_toplevel_icon_manager_v1_interface, 1);
	proto->listener = NULL;

	return proto;
}

static void xdg_toplevel_icon_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	free(data->listener);
	xdg_toplevel_icon_manager_v1_destroy(data->context);
}

/* Standard Wayland event loop. */
static int event_loop(MwLL handle) {
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
		wl_surface_commit(handle->wayland.framebuffer.surface);
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
	do { \
		wayland_protocol_callback_table_t* cb = malloc(sizeof(wayland_protocol_callback_table_t)); \
		cb->setup			      = (wl_setup_func*)interface##_setup; \
		cb->destroy			      = (wl_destroy_func*)interface##_interface_destroy; \
		shput(wayland->wl_protocol_setup_map, interface##_interface.name, cb); \
	} while(0);

	wayland->registry_listener.global	 = new_protocol;
	wayland->registry_listener.global_remove = protocol_removed;
	wayland->wl_protocol_setup_map		 = NULL;
	wayland->wl_protocol_map		 = NULL;

	WL_INTERFACE(wl_shm);
	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(wl_seat);
	WL_INTERFACE(wl_output);
	WL_INTERFACE(wl_data_device_manager);
	WL_INTERFACE(zwp_primary_selection_device_manager_v1);
	if(wayland->type == MWLL_WAYLAND_TOPLEVEL) {
		WL_INTERFACE(xdg_wm_base);
		WL_INTERFACE(zxdg_decoration_manager_v1);
		WL_INTERFACE(xdg_toplevel_icon_manager_v1);
	} else if(wayland->type == MWLL_WAYLAND_POPUP) {
		WL_INTERFACE(xdg_wm_base);
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

	r->wayland.framebuffer.surface = NULL;

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
	r->wayland.framebuffer.surface = wl_compositor_create_surface(r->wayland.compositor);

	r->wayland.toplevel->xdg_surface =
	    xdg_wm_base_get_xdg_surface(WAYLAND_GET_INTERFACE(r->wayland, xdg_wm_base)->context, r->wayland.framebuffer.surface);
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
	wl_surface_commit(r->wayland.framebuffer.surface);
	event_loop(r);
	while(!r->wayland.configured) {
	}

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
}

/* Toplevel destroy function */
static void destroy_toplevel(MwLL r) {
	int i;

	if(shget(r->wayland.wl_protocol_map, zxdg_decoration_manager_v1_interface.name) != NULL) {
		zxdg_decoration_manager_v1_context_t* dec = WAYLAND_GET_INTERFACE(r->wayland, zxdg_decoration_manager_v1)->context;
		zxdg_decoration_manager_v1_destroy(dec->manager);
	}

	wl_surface_destroy(r->wayland.framebuffer.surface);

	wl_compositor_destroy(r->wayland.compositor);

	xdg_surface_destroy(r->wayland.toplevel->xdg_surface);

	xdg_toplevel_destroy(r->wayland.toplevel->xdg_top_level);

	xkb_context_unref(r->wayland.toplevel->xkb_context);

	wl_registry_destroy(r->wayland.registry);

	wl_display_disconnect(r->wayland.display);
}

/* Sublevel setup function */
static void setup_sublevel(MwLL parent, MwLL r, int x, int y) {
	struct wl_compositor* compositor     = parent->wayland.compositor;
	struct wl_surface*    parent_surface = parent->wayland.framebuffer.surface;

	r->wayland.sublevel	    = malloc(sizeof(struct _MwLLWaylandSublevel));
	r->wayland.sublevel->parent = parent;

	if(parent->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		r->wayland.sublevel->parent_xdg_surface = parent->wayland.toplevel->xdg_surface;
	} else {
		r->wayland.sublevel->parent_xdg_surface = parent->wayland.sublevel->parent_xdg_surface;
	}

	r->wayland.type = MWLL_WAYLAND_SUBLEVEL;

	r->wayland.display = parent->wayland.display;

	setup_callbacks(&r->wayland);

	r->wayland.registry = wl_display_get_registry(parent->wayland.display);
	r->wayland.display  = parent->wayland.display;

	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}

	r->wayland.framebuffer.surface = wl_compositor_create_surface(compositor);

	r->wayland.sublevel->subsurface = wl_subcompositor_get_subsurface(r->wayland.sublevel->subcompositor, r->wayland.framebuffer.surface, parent_surface);

	wl_subsurface_set_position(r->wayland.sublevel->subsurface, x, y);

	r->wayland.configured = MwTRUE;
}

/* Sublevel setup function */
static void destroy_sublevel(MwLL r) {
	wl_subsurface_destroy(r->wayland.sublevel->subsurface);

	wl_surface_destroy(r->wayland.framebuffer.surface);

	free(r->wayland.sublevel);
}

static void popup_configure(void*	      data,
			    struct xdg_popup* xdg_popup,
			    int32_t	      x,
			    int32_t	      y,
			    int32_t	      width,
			    int32_t	      height) {
	MwLL self = data;

	self->wayland.x	 = x;
	self->wayland.y	 = y;
	self->wayland.ww = width;
	self->wayland.wh = height;
};

static void popup_done(void*		 data,
		       struct xdg_popup* xdg_popup) {
};

struct xdg_popup_listener popup_listener = {
    .configure	= popup_configure,
    .popup_done = popup_done,
};

/* Popup setup function */
static void setup_popup(MwLL r) {
	int		    i;
	struct xdg_surface* xdg_surface;
	MwLL		    topmost_parent = r->wayland.parent;

	r->wayland.type = MWLL_WAYLAND_POPUP;

	while(topmost_parent->wayland.type != MWLL_WAYLAND_TOPLEVEL) {
		topmost_parent = topmost_parent->wayland.parent;
	}

	r->wayland.popup = malloc(sizeof(struct _MwLLWaylandPopup));
	memset(r->wayland.popup, 0, sizeof(struct _MwLLWaylandPopup));

	setup_callbacks(&r->wayland);

	/* Do a roundtrip to ensure all interfaces are setup. */
	r->wayland.registry = wl_display_get_registry(r->wayland.display);
	wl_registry_add_listener(r->wayland.registry, &r->wayland.registry_listener, r);
	if(wl_display_roundtrip(r->wayland.display) == -1) {
		printf("roundtrip failed\n");
		raise(SIGTRAP);
		return;
	}

	r->wayland.popup->xdg_wm_base = WAYLAND_GET_INTERFACE(r->wayland, xdg_wm_base)->context;

	r->wayland.popup->xdg_positioner = xdg_wm_base_create_positioner(r->wayland.popup->xdg_wm_base);

	xdg_positioner_set_size(r->wayland.popup->xdg_positioner, r->wayland.ww, r->wayland.wh);
	xdg_positioner_set_anchor_rect(
	    r->wayland.popup->xdg_positioner,
	    topmost_parent->wayland.cur_mouse_pos.x,
	    topmost_parent->wayland.cur_mouse_pos.y,
	    1, 1);

	xdg_surface = topmost_parent->wayland.toplevel->xdg_surface;

	r->wayland.framebuffer.surface = wl_compositor_create_surface(r->wayland.compositor);

	r->wayland.popup->xdg_surface =
	    xdg_wm_base_get_xdg_surface(r->wayland.popup->xdg_wm_base, r->wayland.framebuffer.surface);

	r->wayland.popup->xdg_popup = xdg_surface_get_popup(
	    r->wayland.popup->xdg_surface,
	    xdg_surface,
	    r->wayland.popup->xdg_positioner);

	xdg_popup_add_listener(r->wayland.popup->xdg_popup, &popup_listener, r);

	r->wayland.popup->xdg_surface_listener.configure = xdg_surface_configure;

	xdg_surface_add_listener(r->wayland.popup->xdg_surface, &r->wayland.popup->xdg_surface_listener, r);

	/* Perform the initial commit and wait for the first configure event */
	wl_surface_commit(r->wayland.framebuffer.surface);
	event_loop(r);
}

/* Popup destroy function */
static void destroy_popup(MwLL r) {
	xdg_popup_destroy(r->wayland.popup->xdg_popup);

	xdg_surface_destroy(r->wayland.popup->xdg_surface);

	xdg_positioner_destroy(r->wayland.popup->xdg_positioner);

	wl_surface_destroy(r->wayland.framebuffer.surface);

	wl_registry_destroy(r->wayland.registry);
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r;
	r = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
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

	r->wayland.ww	  = width;
	r->wayland.wh	  = height;
	r->wayland.x	  = x;
	r->wayland.y	  = y;
	r->wayland.parent = parent;

	if(parent == NULL) {
		setup_toplevel(r, x, y);
	} else {
		setup_sublevel(parent, r, x, y);
	}

	framebuffer_setup(&r->wayland);

	r->wayland.region = wl_compositor_create_region(r->wayland.compositor);
	region_setup(r);

	MwLLForceRender(r);

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	int	       i;
	struct timeval tv;
	int	       select_ret;

	pthread_mutex_lock(&handle->wayland.eventsMutex);

	MwLLDestroyCommon(handle);

	event_loop(handle);

	framebuffer_destroy(&handle->wayland);
	buffer_destroy(&handle->wayland.cursor);
	wl_region_destroy(handle->wayland.region);

	if(handle->wayland.supports_zwp) {
		zwp_primary_selection_source_v1_destroy(handle->wayland.clipboard_source.zwp);
	} else {
		wl_data_source_destroy(handle->wayland.clipboard_source.wl);
	}
	if(handle->wayland.icon != NULL) {
		buffer_destroy(handle->wayland.icon);
		wl_surface_destroy(handle->wayland.icon->surface);
	}

	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		destroy_toplevel(handle);
	} else if(handle->wayland.type == MWLL_WAYLAND_SUBLEVEL) {
		destroy_sublevel(handle);
	} else if(handle->wayland.type == MWLL_WAYLAND_POPUP) {
		destroy_popup(handle);
	}

	for(i = 0; i < shlen(handle->wayland.wl_protocol_setup_map); i++) {
		void* ctx = shget(handle->wayland.wl_protocol_map, handle->wayland.wl_protocol_setup_map[i].key);

		handle->wayland.wl_protocol_setup_map[i].value->destroy(&handle->wayland, ctx);
		shdel(handle->wayland.wl_protocol_map, handle->wayland.wl_protocol_setup_map[i].value);
	}
	shfree(handle->wayland.wl_protocol_map);
	shfree(handle->wayland.wl_protocol_setup_map);

	if(handle->wayland.pointer != NULL) {
		wl_pointer_destroy(handle->wayland.pointer);
	}
	if(handle->wayland.keyboard != NULL) {
		wl_keyboard_destroy(handle->wayland.keyboard);
	}

	/* sleep long enough that any active attempts to wait for the mutex will have given up and we can safely unlock/destroy it */
	tv.tv_sec  = 0;
	tv.tv_usec = 250;
	do {
		select_ret = select(1, NULL, NULL, NULL, &tv);
	} while((select_ret == -1) && (errno == EINTR));

	pthread_mutex_unlock(&handle->wayland.eventsMutex);
	pthread_mutex_destroy(&handle->wayland.eventsMutex);

	free(handle);
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	*x = handle->wayland.x;
	*y = handle->wayland.y;
	*w = handle->wayland.ww;
	*h = handle->wayland.wh;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	if(handle->wayland.type == MWLL_WAYLAND_SUBLEVEL) {
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

	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL && handle->wayland.configured) {
		xdg_surface_set_window_geometry(handle->wayland.toplevel->xdg_surface, 0, 0, handle->wayland.ww, handle->wayland.wh);
	}

refresh:
	MwLLDispatch(handle, draw, NULL);
	region_setup(handle);

	framebuffer_destroy(&handle->wayland);
	framebuffer_setup(&handle->wayland);
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

	handle->wayland.events_pending += 1;
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

	handle->wayland.events_pending += 1;
}

static void MwLLBeginDrawImpl(MwLL handle) {
}

static void MwLLEndDrawImpl(MwLL handle) {
	update_buffer(&handle->wayland.framebuffer);

	handle->wayland.events_pending += 1;
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
	MwBool		pending = MwFALSE;
	struct timespec timeout;
	int		i;

	if(handle->wayland.always_render) {
		return event_loop(handle);
	}

	timeout.tv_nsec = 10;
	timeout.tv_sec	= 0;

	return handle->wayland.force_render || handle->wayland.events_pending || wl_display_dispatch_timeout(handle->wayland.display, &timeout);
}

static void MwLLNextEventImpl(MwLL handle) {
	if(!handle->wayland.always_render) {
		event_loop(handle);
	}
	if(handle->wayland.events_pending) {
		handle->wayland.events_pending = 0;
	}
	if(handle->wayland.force_render) {
		handle->wayland.force_render = 0;
	}
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
	if(handle->wayland.type == MWLL_WAYLAND_TOPLEVEL) {
		if(WAYLAND_GET_INTERFACE(handle->wayland, xdg_toplevel_icon_manager_v1) != NULL) {
			void*				     map;
			struct xdg_toplevel_icon_manager_v1* icon_manager = WAYLAND_GET_INTERFACE(handle->wayland, xdg_toplevel_icon_manager_v1)->context;
			struct xdg_toplevel_icon_v1*	     icon	  = xdg_toplevel_icon_manager_v1_create_icon(icon_manager);
			MwU64				     size	  = pixmap->common.width * pixmap->common.height * 4;
			int				     i		  = 0;

			if(handle->wayland.icon == NULL) {
				handle->wayland.icon = malloc(sizeof(struct _MwLLWaylandShmBuffer));
				memset(handle->wayland.icon, 0, sizeof(struct _MwLLWaylandShmBuffer));
				handle->wayland.icon->shm = handle->wayland.framebuffer.shm;
			}

			if(handle->wayland.icon->setup) {
				buffer_destroy(handle->wayland.icon);
				wl_surface_destroy(handle->wayland.icon->surface);
			}
			handle->wayland.icon->surface = wl_compositor_create_surface(handle->wayland.compositor);

			buffer_setup(handle->wayland.icon, pixmap->common.width, pixmap->common.height);

			for(; i < size; i += 2) {
				handle->wayland.icon->buf[i]	 = pixmap->common.raw[i + 2];
				handle->wayland.icon->buf[i + 1] = pixmap->common.raw[i + 1];
				handle->wayland.icon->buf[i + 2] = pixmap->common.raw[i + 0];
				handle->wayland.icon->buf[i + 3] = 255;
			}

			update_buffer(handle->wayland.icon);

			xdg_toplevel_icon_v1_add_buffer(icon, handle->wayland.icon->shm_buffer, 1);

			xdg_toplevel_icon_manager_v1_set_icon(icon_manager, handle->wayland.toplevel->xdg_top_level, icon);
		}
	}
}

static void MwLLForceRenderImpl(MwLL handle) {
	wl_surface_damage(handle->wayland.framebuffer.surface, 0, 0, handle->wayland.ww, handle->wayland.wh);

	handle->wayland.force_render = MwTRUE;

	/*
	if(handle->wayland.egl_setup) {
		timed_redraw_by_epoch(handle, 25);
	}
	*/
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	int x, y;

	if(handle->wayland.cursor.setup) {
		buffer_destroy(&handle->wayland.cursor);
		wl_surface_destroy(handle->wayland.cursor.surface);
	}
	buffer_setup(&handle->wayland.cursor, image->width, image->height);
	memset(handle->wayland.cursor.buf, 0, handle->wayland.cursor.buf_size);

	for(y = 0; y < mask->height; y++) {
		unsigned int d = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			int px	= 0;
			int idx = ((y * mask->width) + x) * 4;

			if(d & 1) {
				handle->wayland.cursor.buf[idx + 3] = 255;
			};
			d = d >> 1;
		}
	}
	for(y = 0; y < image->height; y++) {
		unsigned int d = image->data[y];
		for(x = image->width - 1; x >= 0; x--) {
			int px	= 0;
			int idx = ((y * image->width) + x) * 4;

			if(d & 1) {
				px = 255;
			};

			handle->wayland.cursor.buf[idx]	    = px;
			handle->wayland.cursor.buf[idx + 1] = px;
			handle->wayland.cursor.buf[idx + 2] = px;
			d				    = d >> 1;
		}
	}

	handle->wayland.cursor.surface = wl_compositor_create_surface(handle->wayland.compositor);

	wl_surface_attach(handle->wayland.cursor.surface, handle->wayland.cursor.shm_buffer, 0, 0);
	wl_surface_commit(handle->wayland.cursor.surface);

	/* If there's currently a pointer, set it up. (Otherwise, it'll be setup during the pointer enter event) */
	if(handle->wayland.pointer != NULL) {
		wl_pointer_set_cursor(handle->wayland.pointer, handle->wayland.pointer_serial, handle->wayland.cursor.surface, 0, 0);
	}
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	switch(handle->wayland.type) {
	case MWLL_WAYLAND_UNKNOWN:
	case MWLL_WAYLAND_TOPLEVEL:
		destroy_toplevel(handle);
		break;
	case MWLL_WAYLAND_SUBLEVEL:
		destroy_sublevel(handle);
		break;
	case MWLL_WAYLAND_POPUP:
		return;
	}

	switch(handle->wayland.type_to_be) {
	case MWLL_WAYLAND_POPUP:
		setup_popup(handle);
		return;
	default:
		setup_toplevel(handle, point->x, point->y);
		break;
	}
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
	printf("[WARNING] MwLLFocus not supported on Wayland\n");
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text) {
	if(handle->wayland.clipboard_buffer != NULL) {
		free(handle->wayland.clipboard_buffer);
	}
	handle->wayland.clipboard_buffer = malloc(strlen(text));
	strcpy(handle->wayland.clipboard_buffer, text);

	if(handle->wayland.supports_zwp) {
		for(int i = 0; i < arrlen(handle->wayland.clipboard_devices); i++) {
			wl_clipboard_device_context_t* device = handle->wayland.clipboard_devices[i];
			zwp_primary_selection_device_v1_set_selection(device->device.zwp, handle->wayland.clipboard_source.zwp, handle->wayland.keyboard_serial);
		}
	} else {
		for(int i = 0; i < arrlen(handle->wayland.clipboard_devices); i++) {
			wl_clipboard_device_context_t* device = handle->wayland.clipboard_devices[i];
			wl_data_device_set_selection(device->device.wl, handle->wayland.clipboard_source.wl, handle->wayland.keyboard_serial);
		}
	}
}

static void MwLLGetClipboardImpl(MwLL handle) {
	/* no-op */
	return;
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	handle->wayland.type_to_be = MWLL_WAYLAND_POPUP;
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	*point = handle->wayland.cur_mouse_pos;
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	rect->x	     = 0;
	rect->y	     = 0;
	rect->width  = handle->wayland.mw;
	rect->height = handle->wayland.mh;
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

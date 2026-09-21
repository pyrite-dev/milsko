#include <Mw/Milsko.h>
#include <sys/mman.h>
#include "../../../external/stb_ds.h"

/* TODO: find out what FreeBSD and such wants us to include */
#ifdef __FreeBSD__
/* XXX: Untested (nishi) */
#include <evdev/input.h>
#else
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

#include <stdbool.h>
#include <sys/poll.h>

static void setup_clipboard(MwLL self, struct wl_seat* wl_seat);
static void setup_zwp_clipboard(MwLL self, struct wl_seat* wl_seat);

static void   destroy_clipboard(MwLL self, struct wl_seat* wl_seat);
static void   destroy_zwp_clipboard(MwLL self, struct wl_seat* wl_seat);
static MwBool hit_detect(MwLL child, MwLL* _topmost_parent, MwPoint* _point, MwPoint* _relative_mouse_pos, MwPoint* _absolute_pos);

/* Recursively dispatch a key event to a widget and its children */
static void recursive_dispatch_key(MwLL handle, int* k) {
	MwWidget h = (MwWidget)handle->common.user;
	MwLLDispatch(handle, key, k);
	if(h) {
		int i;
		for(i = 0; i < arrlen(h->children); i++) {
			MwLLDispatch(h->children[i]->lowlevel, key, k);
			if(arrlen(h->children[i]->children) > 0) {
				recursive_dispatch_key(h->children[i]->lowlevel, k);
			}
		}
	}
};
/* Recursively dispatch a key released event to a widget and its children */
static void recursive_dispatch_key_released(MwLL handle, int* k) {
	MwWidget h = (MwWidget)handle->common.user;
	MwLLDispatch(handle, key_released, k);
	if(h) {
		int i;
		for(i = 0; i < arrlen(h->children); i++) {
			MwLLDispatch(h->children[i]->lowlevel, key_released, k);
			if(arrlen(h->children[i]->children) > 0) {
				recursive_dispatch_key_released(h->children[i]->lowlevel, k);
			}
		}
	}
};

/* Recursively dispatch a move event to a widget and its children */
static void recursive_dispatch_move(MwLL handle, MwMouse* p) {
	MwWidget h = (MwWidget)handle->common.user;
	if(h) {
		int i;
		for(i = 0; i < arrlen(h->children); i++) {
			MwLLDispatch(h->children[i]->lowlevel, move, p);
			if(arrlen(h->children[i]->children) > 0) {
				recursive_dispatch_move(h->children[i]->lowlevel, p);
			}
		}
	}
};

/* `wl_registry.global` callback */
static void new_protocol(void* data, struct wl_registry* registry,
			 MwU32 name, const char* interface,
			 MwU32 version) {
	MwLL self = data;
	(void)version;
	(void)registry;

	WAYLAND_EVENT_OP_START(self);

	wayland_protocol_callback_table_t* cb = shget(self->wayland.wl_protocol_setup_map, interface);
	if(cb != NULL) {
		shput(self->wayland.wl_protocol_map, interface, cb->setup(name, data, version));
		/* we don't care for adding this protocol, we just use it to know if the compositor will let us have transparent surfaces */
	} else if(strcmp(interface, "wp_alpha_modifier_v1") == 0) {
		self->common.supports_transparency = MwTRUE;
	} else {
		// printf("unknown interface %s\n", interface);
	}

	WAYLAND_EVENT_OP_END(self);
};

/* `wl_registry.global_remove` callback */
static void protocol_removed(void*		 data,
			     struct wl_registry* registry,
			     MwU32		 name) {
	(void)data;
	(void)registry;
	(void)name;
	printf("%d destroyed\n", name);
};

static void wl_offer(void*		   data,
		     struct wl_data_offer* wl_data_offer,
		     const char*	   mime_type) {
	wl_clipboard_device_context_t* self = data;
	int			       i;
	for(i = 0; i < sizeof(self->accepted_types) / sizeof(self->accepted_types[0]); i++) {
		if(strcmp(self->accepted_types[i], mime_type) == 0) {
			wl_data_offer_accept(wl_data_offer, self->ll->wayland.clipboard_serial, mime_type);
			memset(self->selected_mime_type, 0, 4096);
			snprintf(self->selected_mime_type, sizeof(self->selected_mime_type), "%s", mime_type);
			break;
		}
	}
};

static void wl_source_actions(void*		    data,
			      struct wl_data_offer* wl_data_offer,
			      uint32_t		    source_actions) {

	// printf("%d\n", source_actions);
};

static void wl_action(void*		    data,
		      struct wl_data_offer* wl_data_offer,
		      uint32_t		    dnd_action) {};

struct wl_data_offer_listener offer_listener = {
    .offer	    = wl_offer,
    .source_actions = wl_source_actions,
    .action	    = wl_action,
};

static void wl_data_device_data_offer(void*		     data,
				      struct wl_data_device* wl_data_device,
				      struct wl_data_offer*  offer) {
	wl_clipboard_device_context_t* self = data;
	(void)wl_data_device;

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
	wl_clipboard_device_context_t* self = data;
	(void)wl_data_device;
	(void)surface;
	(void)x;
	(void)y;
	(void)id;

	WAYLAND_EVENT_OP_START(self);

	self->ll->wayland.clipboard_serial = serial;

	WAYLAND_EVENT_OP_END(self);
};
static void wl_data_device_leave(void*			data,
				 struct wl_data_device* wl_data_device) {
	wl_clipboard_device_context_t* self = data;

	if(self->offer.wl) {
		wl_data_offer_destroy(self->offer.wl);
		self->offer.wl = NULL;
	}
};

static MwBool motion_cursor_change(MwLL self, wl_clipboard_device_context_t* ctx) {
	if(self->wayland.type == MwLL_WAYLAND_TOPLEVEL && self->wayland.accepts_dnd) {
		return MwTRUE;
	} else if(self->common.user) {
		MwWidget w = self->common.user;
		int	 i, n;
		for(i = 0; i < arrlen(w->children); i++) {
			if(w->children[i]->lowlevel->wayland.type == MwLL_WAYLAND_SUBLEVEL) {
				MwLL	child	       = w->children[i]->lowlevel;
				MwLL	topmost_parent = child;
				MwPoint point;
				MwPoint relative_mouse_pos;
				MwPoint absolute_pos;

				if(hit_detect(child, &topmost_parent, &point, &relative_mouse_pos, &absolute_pos)) {
					return MwTRUE;
				} else {
					return motion_cursor_change(child, ctx);
				}
			}
		}
	}
	return MwFALSE;
}

static void clipboard_dispatch(MwLL self, char* buf) {
	if(self->wayland.type == MwLL_WAYLAND_TOPLEVEL && self->wayland.accepts_dnd) {
		MwLLDispatch(self, drag_and_drop, buf);
	} else if(self->common.user) {
		MwWidget w = self->common.user;
		int	 i, n;
		for(i = 0; i < arrlen(w->children); i++) {
			if(w->children[i]->lowlevel->wayland.type == MwLL_WAYLAND_SUBLEVEL) {
				MwLL	child	       = w->children[i]->lowlevel;
				MwLL	topmost_parent = child;
				MwPoint point;
				MwPoint relative_mouse_pos;
				MwPoint absolute_pos;

				if(hit_detect(child, &topmost_parent, &point, &relative_mouse_pos, &absolute_pos) && child->wayland.accepts_dnd) {
					MwLLDispatch(child, drag_and_drop, buf);
					return;
				} else {
					clipboard_dispatch(child, buf);
				}
			}
		}
	}
}

static void wl_data_device_motion(void*			 data,
				  struct wl_data_device* wl_data_device,
				  uint32_t		 time,
				  wl_fixed_t		 x,
				  wl_fixed_t		 y) {
	(void)wl_data_device;
	(void)time;
	(void)x;
	(void)y;
	wl_clipboard_device_context_t* self = data;

	self->ll->wayland.cur_mouse_pos.x = wl_fixed_to_int(x);
	self->ll->wayland.cur_mouse_pos.y = wl_fixed_to_int(y);

	if(self->selected_mime_type[0] != '\0' && self->offer.wl) {
		if(motion_cursor_change(self->ll, self)) {
			wl_data_offer_set_actions(self->offer.wl, WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY, WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY);
		} else {
			wl_data_offer_set_actions(self->offer.wl, WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE, WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE);
		};
	}
};
static void wl_data_device_drop(void*		       data,
				struct wl_data_device* wl_data_device) {
	wl_clipboard_device_context_t* self = data;
	int			       fds[2];
	fd_set			       set;
	char*			       buf = NULL;
	struct timeval		       timeout;
	int			       rc = 0;

	if(pipe(fds) != 0) {
		return;
	}

	FD_ZERO(&set);
	FD_SET(fds[0], &set);
	timeout.tv_sec	= 0;
	timeout.tv_usec = 1000;

	wl_data_offer_receive(self->offer.wl, self->selected_mime_type, fds[1]);
	close(fds[1]);

	MwLLWaylandFlush(self->ll);
	wl_display_roundtrip(self->ll->wayland.display);

	while(MwTRUE) {
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

	setup_clipboard(self->ll, self->ll->wayland.pointer_seat);

	char* buf_ptr = buf;
	while(buf_ptr) {
		char* next = strstr(buf_ptr, "\n");
		if(next) {
			*next = '\0';
			if(strstr(buf_ptr, "file://")) {
				clipboard_dispatch(self->ll, buf_ptr + sizeof("file://") - 1);
			} else {
				clipboard_dispatch(self->ll, buf_ptr);
			}
			buf_ptr = next + 1;
		} else {
			buf_ptr = NULL;
		}
	}

	arrfree(buf);

	self->selected_mime_type[0] = '\0';
};

static void wl_data_device_selection(void*		    data,
				     struct wl_data_device* wl_data_device,
				     struct wl_data_offer*  id) {
	(void)data;
	(void)wl_data_device;
	(void)id;
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
	(void)zwp_primary_selection_device_v1;

	self->offer.zwp = offer;
};

static void zwp_primary_selection_device_v1_selection(void*				      data,
						      struct zwp_primary_selection_device_v1* zwp_primary_selection_device_v1,
						      struct zwp_primary_selection_offer_v1*  id) {
	(void)data;
	(void)zwp_primary_selection_device_v1;
	(void)id;
};

struct zwp_primary_selection_device_v1_listener zwp_primary_selection_device_v1_listener = {
    .data_offer = zwp_primary_selection_device_v1_data_offer,
    .selection	= zwp_primary_selection_device_v1_selection,
};

void MwLLWaylandClipboardRead(wl_clipboard_device_context_t* ctx, int clipboard_type) {
	int	       fds[2];
	fd_set	       set;
	char*	       buf = NULL;
	struct timeval timeout;
	int	       rc = 0;

	if(pipe(fds) != 0) {
		return;
	}

	FD_ZERO(&set);
	FD_SET(fds[0], &set);
	timeout.tv_sec	= 0;
	timeout.tv_usec = 100;

	if(clipboard_type == MwCLIPBOARD_PRIMARY && ctx->offer.zwp) {
		zwp_primary_selection_offer_v1_receive(ctx->offer.zwp, "text/plain", fds[1]);
	} else if(ctx->offer.wl) {
		wl_data_offer_receive(ctx->offer.wl, "text/plain", fds[1]);
	}
	close(fds[1]);

	MwLLWaylandFlush(ctx->ll);
	wl_display_roundtrip(ctx->ll->wayland.display);

	while(MwTRUE) {
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

	ctx->ll->wayland.events_pending = 1;
}

static void wl_data_source_listener_target(void*		  data,
					   struct wl_data_source* wl_data_source,
					   const char*		  mime_type) {
	(void)data;
	(void)wl_data_source;
	(void)mime_type;
};
static void wl_data_source_listener_send(void*			data,
					 struct wl_data_source* wl_data_source,
					 const char*		mime_type,
					 int32_t		fd) {
	MwLL self = data;
	(void)wl_data_source;
	(void)mime_type;

	WAYLAND_EVENT_OP_START(self);
	if(self->wayland.clipboard_buffer != NULL) {
		write(fd, self->wayland.clipboard_buffer, strlen(self->wayland.clipboard_buffer));
		close(fd);
	}
	WAYLAND_EVENT_OP_END(self);
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

	WAYLAND_EVENT_OP_START(self);

	if(wl_data_source)
		wl_data_source_destroy(wl_data_source);

	self->wayland.clipboard_source.wl = wl_data_device_manager_create_data_source(self->wayland.clipboard_manager.wl);

	wl_data_source_offer(self->wayland.clipboard_source.wl, "text/plain");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "text/plain;charset=utf-8");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "TEXT");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "STRING");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "UTF8_STRING");
	wl_data_source_offer(self->wayland.clipboard_source.wl, "text/uri-list");

	wl_data_source_add_listener(self->wayland.clipboard_source.wl, &wl_data_source_listener, self);

	WAYLAND_EVENT_OP_END(self);
};

static void zwp_primary_selection_source_v1_send(void*					 data,
						 struct zwp_primary_selection_source_v1* wl_data_source,
						 const char*				 mime_type,
						 int32_t				 fd) {
	MwLL self = data;
	(void)wl_data_source;
	(void)mime_type;

	WAYLAND_EVENT_OP_START(self);

	if(self->wayland.clipboard_buffer != NULL) {
		write(fd, self->wayland.clipboard_buffer, strlen(self->wayland.clipboard_buffer));
		close(fd);
	}

	WAYLAND_EVENT_OP_END(self);
};
static void zwp_primary_selection_source_v1_cancelled(void*				      data,
						      struct zwp_primary_selection_source_v1* wl_data_source) {
	MwLL self = data;
	(void)wl_data_source;

	WAYLAND_EVENT_OP_START(self);

	zwp_primary_selection_source_v1_destroy(self->wayland.clipboard_source.zwp);

	self->wayland.clipboard_source.zwp = zwp_primary_selection_device_manager_v1_create_source(self->wayland.clipboard_manager.zwp);

	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "text/plain");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "text/plain;charset=utf-8");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "TEXT");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "STRING");
	zwp_primary_selection_source_v1_offer(self->wayland.clipboard_source.zwp, "UTF8_STRING");

	WAYLAND_EVENT_OP_END(self);
};

struct zwp_primary_selection_source_v1_listener zwp_primary_selection_source_v1_listener = {
    .send      = zwp_primary_selection_source_v1_send,
    .cancelled = zwp_primary_selection_source_v1_cancelled,
};

/* wl_data_device_manager setup function */
static wayland_protocol_t* wl_data_device_manager_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->clipboard_manager.wl = wl_registry_bind(wayland->registry, name, &wl_data_device_manager_interface, 3);

	wayland->clipboard_source.wl = wl_data_device_manager_create_data_source(wayland->clipboard_manager.wl);

	wl_data_source_offer(wayland->clipboard_source.wl, "text/plain");
	wl_data_source_offer(wayland->clipboard_source.wl, "text/plain;charset=utf-8");
	wl_data_source_offer(wayland->clipboard_source.wl, "TEXT");
	wl_data_source_offer(wayland->clipboard_source.wl, "STRING");
	wl_data_source_offer(wayland->clipboard_source.wl, "UTF8_STRING");
	wl_data_source_offer(wayland->clipboard_source.wl, "text/uri-list");

	wl_data_source_add_listener(wayland->clipboard_source.wl, &wl_data_source_listener, wayland);

	if(wayland->pointer_seat) {
		setup_clipboard((MwLL)wayland, wayland->pointer_seat);
	}

	return NULL;
}

static void wl_data_device_manager_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)data;
	if(wayland->clipboard_manager.wl != NULL) {
		wl_data_device_manager_destroy(wayland->clipboard_manager.wl);
		wayland->clipboard_manager.wl = NULL;
	}
	if(wayland->clipboard_source.wl) {
		// wl_data_source_destroy(wayland->clipboard_source.wl);
		wayland->clipboard_source.wl = NULL;
	}
}

/* zwp_primary_selection_device_manager_v1 setup function */
static wayland_protocol_t* zwp_primary_selection_device_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->clipboard_manager.zwp = wl_registry_bind(wayland->registry, name, &zwp_primary_selection_device_manager_v1_interface, 1);

	wayland->clipboard_source.zwp = zwp_primary_selection_device_manager_v1_create_source(wayland->clipboard_manager.zwp);

	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "text/plain");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "text/plain;charset=utf-8");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "TEXT");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "STRING");
	zwp_primary_selection_source_v1_offer(wayland->clipboard_source.zwp, "UTF8_STRING");

	zwp_primary_selection_source_v1_add_listener(wayland->clipboard_source.zwp, &zwp_primary_selection_source_v1_listener, wayland);

	wayland->supports_zwp = MwTRUE;

	if(wayland->pointer_seat) {
		setup_zwp_clipboard((MwLL)wayland, wayland->pointer_seat);
	}

	return NULL;
}

static void zwp_primary_selection_device_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

/* zwp_primary_selection_device_manager_v1 setup function */
static wayland_protocol_t* zwp_pointer_constraints_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->pointer_constraints = wl_registry_bind(wayland->registry, name, &zwp_pointer_constraints_v1_interface, 1);
	return NULL;
}

static void zwp_pointer_constraints_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

static struct wl_surface* curSurface = NULL;

/* `wl_pointer.enter` callback */
static void pointer_enter(void* data, struct wl_pointer* wl_pointer, MwU32 serial,
			  struct wl_surface* surface, wl_fixed_t surface_x,
			  wl_fixed_t surface_y) {
	MwLL self	    = data;
	MwLL topmost_parent = self;

	(void)surface_x;
	(void)surface_y;
	(void)wl_pointer;

	WAYLAND_EVENT_OP_START(self);
	while(topmost_parent->wayland.parent) topmost_parent = topmost_parent->wayland.parent;

	if(self->wayland.framebuffer.surface == surface) {
		curSurface		     = surface;
		self->wayland.pointer_serial = serial;

		wl_pointer_set_cursor(self->wayland.pointer, self->wayland.pointer_serial, topmost_parent->wayland.cursor.surface, 0, 0);
	}

	if(self->wayland.backbuffer.surface == surface) {
		curSurface = surface;
	}

	self->wayland.cur_mouse_pos.x = wl_fixed_to_int(surface_x);
	self->wayland.cur_mouse_pos.y = wl_fixed_to_int(surface_y);

	WAYLAND_EVENT_OP_END(self);
};

/* `wl_pointer.leave` callback */
static void pointer_leave(void* data, struct wl_pointer* wl_pointer, MwU32 serial,
			  struct wl_surface* surface) {
	MwLL self = data;
	(void)wl_pointer;
	(void)serial;
	(void)surface;
	WAYLAND_EVENT_OP_START(self);
	curSurface = NULL;
	WAYLAND_EVENT_OP_END(self);
};

static void xdg_borderless_step_mdown(MwLL self, MwMouse p, MwU32 serial) {
	if(self->wayland.type == MwLL_WAYLAND_TOPLEVEL && self->wayland.backbuffer.surface == curSurface) {
		if(p.point.y >= (self->wayland.wh + CSD_BORDER_FRAME_TOP + CSD_BORDER_FRAME_BOTTOM) - 5) {
			if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 5) {
				xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM_RIGHT);
			} else {
				xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_BOTTOM);
			}
		} else if(p.point.y <= 5) {
			if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 5) {
				xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_TOP_RIGHT);
			} else {
				xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_TOP);
			}
		} else if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 5) {
			xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_RIGHT);
		} else if(p.point.x <= 5) {
			xdg_toplevel_resize(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial, XDG_TOPLEVEL_RESIZE_EDGE_LEFT);
		} else if(p.point.y <= CSD_BORDER_FRAME_TOP) {
			if(p.point.x >= CSD_BORDER_FRAME_LEFT && p.point.x <= CSD_BORDER_FRAME_LEFT + 18) {
			} else if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 21) {
			} else if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 38) {
			} else {
				xdg_toplevel_move(self->wayland.toplevel->xdg_top_level, self->wayland.pointer_seat, serial);
			}
		}
	};
}
static void xdg_borderless_step_mup(MwLL self, MwMouse p, MwU32 serial) {
	(void)serial;
	if(self->wayland.type == MwLL_WAYLAND_TOPLEVEL && self->wayland.backbuffer.surface == curSurface) {
		if(p.point.y <= CSD_BORDER_FRAME_TOP) {
			if(p.point.x >= CSD_BORDER_FRAME_LEFT && p.point.x <= CSD_BORDER_FRAME_LEFT + 18) {
				MwLLDispatch(self, close, NULL);
			} else if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 21) {
				self->wayland.toplevel->maxim_state = !self->wayland.toplevel->maxim_state;
				if(self->wayland.toplevel->maxim_state == 0) {
					xdg_toplevel_unset_maximized(self->wayland.toplevel->xdg_top_level);
				} else {
					xdg_toplevel_set_maximized(self->wayland.toplevel->xdg_top_level);
				}
				self->wayland.setting_wh = 1;
				MwLLWaylandRegionSetup(self);
			} else if(p.point.x >= (self->wayland.ww + CSD_BORDER_FRAME_LEFT + CSD_BORDER_FRAME_RIGHT) - 38) {
				xdg_toplevel_set_minimized(self->wayland.toplevel->xdg_top_level);
			}
		}
	};
}

static void relative_pointer_motion(void*			    data,
				    struct zwp_relative_pointer_v1* pointer,
				    uint32_t			    timeHi,
				    uint32_t			    timeLo,
				    wl_fixed_t			    dx,
				    wl_fixed_t			    dy,
				    wl_fixed_t			    dxUnaccel,
				    wl_fixed_t			    dyUnaccel) {
	MwLL	self = data;
	MwMouse p;

	(void)pointer;
	(void)timeHi;
	(void)timeLo;
	(void)dx;
	(void)dy;

	WAYLAND_EVENT_OP_START(self);

	p.point.x = wl_fixed_to_int(dxUnaccel);
	p.point.y = wl_fixed_to_int(dyUnaccel);

	recursive_dispatch_move(self, &p);
	if(self->wayland.locked_pointer) zwp_locked_pointer_v1_set_cursor_position_hint(self->wayland.locked_pointer, 0, CSD_BORDER_FRAME_TOP);

	WAYLAND_EVENT_OP_END(self);
}

struct zwp_relative_pointer_v1_listener MwLLWaylandRelativePointerListener =
    {
	relative_pointer_motion};

/* zwp_primary_selection_device_manager_v1 setup function */
static wayland_protocol_t* zwp_relative_pointer_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->relative_pointer_manager = wl_registry_bind(wayland->registry, name, &zwp_relative_pointer_manager_v1_interface, 1);
	return NULL;
}

static void zwp_relative_pointer_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

/* `wl_pointer.motion` callback */
static void pointer_motion(void* data, struct wl_pointer* wl_pointer, MwU32 time,
			   wl_fixed_t surface_x, wl_fixed_t surface_y) {
	MwLL	self	       = data;
	MwLL	topmost_parent = self;
	MwMouse p;
	MwBool	inArea		     = MwFALSE;
	MwLL*	currentlyHeldWidgets = NULL;
	int	i;

	(void)time;
	(void)wl_pointer;

	WAYLAND_EVENT_OP_START(self);
	while(topmost_parent->wayland.parent) topmost_parent = topmost_parent->wayland.parent;

	currentlyHeldWidgets = topmost_parent->wayland.currentlyHeldWidgets;

	if(self->wayland.framebuffer.surface) {
		inArea |= self->wayland.framebuffer.surface == curSurface;
	}

	// if(self->wayland.backbuffer.surface) {
	// 	inArea |= self->wayland.backbuffer.surface == curSurface;
	// }

	self->wayland.cur_mouse_pos.x = wl_fixed_to_int(surface_x);
	self->wayland.cur_mouse_pos.y = wl_fixed_to_int(surface_y);

	if(inArea) {
		p.point = self->wayland.cur_mouse_pos;
		MwLLDispatch(self, move, &p);
		wl_pointer_set_cursor(self->wayland.pointer, self->wayland.pointer_serial, self->wayland.cursor.surface, 0, 0);

		for(i = 0; i < arrlen(currentlyHeldWidgets); i++) {
			MwLL new_topmost = currentlyHeldWidgets[i];
			p.point		 = topmost_parent->wayland.cur_mouse_pos;
			while(new_topmost->wayland.parent) {
				p.point.x -= new_topmost->wayland.x;
				p.point.y -= new_topmost->wayland.y;
				new_topmost = new_topmost->wayland.parent;
			}

			MwLLDispatch(currentlyHeldWidgets[i], move, &p);
		}
	}
	if(self->wayland.backbuffer.surface) {
		wl_pointer_set_cursor(self->wayland.pointer, self->wayland.pointer_serial, self->wayland.cursor.surface, 0, 0);
	}

	WAYLAND_EVENT_OP_END(self);
};

static void recursive_dispatch_mouse_down(MwLL handle, MwMouse* p) {
	MwWidget h = (MwWidget)handle->common.user;
	MwLLDispatch(handle, down, p);
	if(h) {
		int i;
		for(i = 0; i < arrlen(h->children); i++) {
			MwLLDispatch(h->children[i]->lowlevel, down, p);
			if(arrlen(h->children[i]->children) > 0) {
				recursive_dispatch_mouse_down(h->children[i]->lowlevel, p);
			}
		}
	}
};
static void recursive_dispatch_mouse_up(MwLL handle, MwMouse* p) {
	MwWidget h = (MwWidget)handle->common.user;
	MwLLDispatch(handle, up, p);
	if(h) {
		int i;
		for(i = 0; i < arrlen(h->children); i++) {
			MwLLDispatch(h->children[i]->lowlevel, up, p);
			if(arrlen(h->children[i]->children) > 0) {
				recursive_dispatch_mouse_up(h->children[i]->lowlevel, p);
			}
		}
	}
};

static MwBool hit_detect(MwLL child, MwLL* _topmost_parent, MwPoint* _point, MwPoint* _relative_mouse_pos, MwPoint* _absolute_pos) {
	MwLL	topmost_parent = child;
	MwPoint point;
	MwPoint relative_mouse_pos;
	MwPoint absolute_pos;

	absolute_pos.x = child->wayland.x;
	absolute_pos.y = child->wayland.y;
	while(topmost_parent->wayland.parent) {
		topmost_parent = topmost_parent->wayland.parent;
		if(topmost_parent) {
			/* if the topmost parent is a popup then its x/y is irrelevant to us.
			 * it's gonna be the position of the popup itself, and we want our
			 * position to be relative to the popup itself. */
			if(topmost_parent->wayland.type != MwLL_WAYLAND_POPUP) {
				if(topmost_parent->wayland.x > 0)
					absolute_pos.x += topmost_parent->wayland.x;
				if(topmost_parent->wayland.y > 0)
					absolute_pos.y += topmost_parent->wayland.y;
			}
		}
	}

	point = topmost_parent->wayland.cur_mouse_pos;

	relative_mouse_pos.x = point.x - absolute_pos.x;
	relative_mouse_pos.y = point.y - absolute_pos.y;

	*_topmost_parent     = topmost_parent;
	*_point		     = point;
	*_relative_mouse_pos = relative_mouse_pos;
	*_absolute_pos	     = absolute_pos;

	return point.x >= absolute_pos.x && point.x <= absolute_pos.x + child->wayland.ww &&
	       point.y >= absolute_pos.y && point.y <= absolute_pos.y + child->wayland.wh;
}

static void mouse_dispatch(MwLL self, MwMouse p, MwU32 state) {
	switch(state) {
	case WL_POINTER_BUTTON_STATE_PRESSED:
		MwLLDispatch(self, down, &p);
		break;
	case WL_POINTER_BUTTON_STATE_RELEASED:
		MwLLDispatch(self, up, &p);
		break;
	}

	if(self->common.user) {
		MwWidget w = self->common.user;
		int	 i, n;
		for(i = 0; i < arrlen(w->children); i++) {
			if(w->children[i]->lowlevel->wayland.type == MwLL_WAYLAND_SUBLEVEL) {
				MwLL	child	       = w->children[i]->lowlevel;
				MwLL	topmost_parent = child;
				MwPoint point;
				MwPoint relative_mouse_pos;
				MwPoint absolute_pos;

				if(hit_detect(child, &topmost_parent, &point, &relative_mouse_pos, &absolute_pos)) {
					int idx = -1;
					for(n = 0; n < arrlen(topmost_parent->wayland.currentlyHeldWidgets); n++) {
						if(topmost_parent->wayland.currentlyHeldWidgets[n] == child) {
							idx = n;
						}
					}
					p.point = relative_mouse_pos;

					switch(state) {
					case WL_POINTER_BUTTON_STATE_PRESSED:
						if(idx == -1) {
							arrpush(topmost_parent->wayland.currentlyHeldWidgets, child);
						}
						break;
					case WL_POINTER_BUTTON_STATE_RELEASED:
						if(idx != -1) {
							arrdel(topmost_parent->wayland.currentlyHeldWidgets, idx);
						}
						break;
					}

					mouse_dispatch(child, p, state);
				}
			}
		}
	}
}

/* `wl_pointer.button` callback */
static void pointer_button(void* data, struct wl_pointer* wl_pointer, MwU32 serial, MwU32 time, MwU32 button, MwU32 state) {
	MwLL	self = data;
	MwMouse p;
	MwLL	topmost_parent = self;
	MwBool	inArea	       = MwFALSE;

	(void)wl_pointer;
	(void)serial;
	(void)time;

	WAYLAND_EVENT_OP_START(self);

	if(self->wayland.type != MwLL_WAYLAND_POPUP) {
		while(topmost_parent->wayland.parent) topmost_parent = topmost_parent->wayland.parent;
	}

	p.point = self->wayland.cur_mouse_pos;

	if(self->wayland.framebuffer.surface) {
		inArea |= self->wayland.framebuffer.surface == curSurface;
	}

	if(inArea) {
		switch(button) {
		case BTN_LEFT:
			p.button = MwMOUSE_LEFT;
			break;
		case BTN_MIDDLE:
			p.button = MwMOUSE_MIDDLE;
			break;
		case BTN_RIGHT:
			p.button = MwMOUSE_RIGHT;
			break;
		}
		self->wayland.held_down = state == WL_POINTER_BUTTON_STATE_PRESSED;

		arrsetlen(self->wayland.currentlyHeldWidgets, 0);
		mouse_dispatch(self, p, state);
	}
	if(self->wayland.backbuffer.surface) {
		if(!self->wayland.has_decorations && self->wayland.do_csd) {
			if(state != WL_POINTER_BUTTON_STATE_RELEASED)
				xdg_borderless_step_mdown(self, p, serial);
			else
				xdg_borderless_step_mup(self, p, serial);
		}
	}

	MwLLForceRender(self);

	WAYLAND_EVENT_OP_END(self);
};

/* `wl_pointer.axis` callback */
static void pointer_axis(void* data, struct wl_pointer* wl_pointer, MwU32 time,
			 MwU32 axis, wl_fixed_t value) {
	(void)data;
	(void)wl_pointer;
	(void)time;
	(void)axis;
	(void)value;
};

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
	(void)wl_keyboard;

	if(!self->wayland.parent) {
		assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

		char* map_shm = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
		assert(map_shm != MAP_FAILED);

		struct xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(
		    self->wayland.xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1,
		    XKB_KEYMAP_COMPILE_NO_FLAGS);
		munmap(map_shm, size);
		close(fd);

		struct xkb_state* xkb_state = xkb_state_new(xkb_keymap);

		self->wayland.xkb_keymap = xkb_keymap;
		self->wayland.xkb_state	 = xkb_state;
	} else {
		self->wayland.xkb_keymap = self->wayland.parent->wayland.xkb_keymap;
		self->wayland.xkb_state	 = self->wayland.parent->wayland.xkb_state;
	}
};

/* `wl_keyboard.enter` callback */
static void keyboard_enter(void*	       data,
			   struct wl_keyboard* wl_keyboard,
			   MwU32	       serial,
			   struct wl_surface*  surface,
			   struct wl_array*    keys) {
	MwLL self = data;
	(void)wl_keyboard;
	(void)surface;
	(void)keys;

	WAYLAND_EVENT_OP_START(self);

	self->wayland.keyboard_serial = serial;

	MwLLDispatch(self, focus_in, NULL);

	WAYLAND_EVENT_OP_END(self);
};

/* `wl_keyboard.leave` callback */
static void keyboard_leave(void*	       data,
			   struct wl_keyboard* wl_keyboard,
			   MwU32	       serial,
			   struct wl_surface*  surface) {
	MwLL self = data;
	(void)wl_keyboard;
	(void)serial;
	(void)surface;

	WAYLAND_EVENT_OP_START(self);

	MwLLDispatch(self, focus_out, NULL);

	WAYLAND_EVENT_OP_END(self);
};

void MwLLRecursiveKeyDispatch(MwLL self, int* k, MwBool down) {
	if(self->common.user) {
		MwWidget w = self->common.user;
		int	 i, n;
		for(i = 0; i < arrlen(w->children); i++) {
			if(w->children[i]->lowlevel->wayland.type == MwLL_WAYLAND_SUBLEVEL) {
				MwLL	child	       = w->children[i]->lowlevel;
				MwLL	topmost_parent = child;
				MwPoint point;
				MwPoint relative_mouse_pos;
				MwPoint absolute_pos;

				if(hit_detect(child, &topmost_parent, &point, &relative_mouse_pos, &absolute_pos)) {
					if(down) {
						MwLLDispatch(child, key, k);
					} else {
						MwLLDispatch(child, key_released, k);
					}

					MwLLRecursiveKeyDispatch(child, k, down);
				}
			}
		}
	}
}

/* `wl_keyboard.key` callback */
static void keyboard_key(void*		     data,
			 struct wl_keyboard* wl_keyboard,
			 MwU32		     serial,
			 MwU32		     _time,
			 MwU32		     key,
			 MwU32		     state) {
	MwLL   self	      = data;
	MwBool inArea	      = 0;
	MwLL   topmost_parent = self;

	(void)wl_keyboard;
	(void)serial;
	(void)_time;

	while(topmost_parent->wayland.parent) topmost_parent = topmost_parent->wayland.parent;

	WAYLAND_EVENT_OP_START(self);

	if(self->wayland.framebuffer.surface) {
		inArea |= self->wayland.framebuffer.surface == curSurface;
	}

	inArea |= (self == topmost_parent->wayland.focusedWidget);

	if(inArea) {

		xkb_layout_index_t  layout;
		MwU32		    levels;
		xkb_level_index_t   level = 0;
		const xkb_keysym_t* syms_out;
		MwU32		    keycode = key + 8;
		MwU64		    syms_num;
		int		    i;

		if(!self->wayland.xkb_keymap) {
			WAYLAND_EVENT_OP_END(self);
			return;
		}

		layout = xkb_state_key_get_layout(self->wayland.xkb_state, keycode);
		levels =
		    xkb_keymap_num_levels_for_key(self->wayland.xkb_keymap, keycode, layout);

		if((((self->wayland.mod_state & 1) == 1) || ((self->wayland.mod_state & 2) == 2)) && levels >= 2) {
			level = 1;
		} else if(levels >= 1) {
			level = 0;
		}
		syms_num = xkb_keymap_key_get_syms_by_level(self->wayland.xkb_keymap, keycode, layout, level, &syms_out);
		if(syms_out == NULL) {
			WAYLAND_EVENT_OP_END(self);
			return;
		}

		for(i = 0; i < syms_num; i++) {
			int sym = syms_out[i];
			int key = -1;

			switch(sym) {
			case XKB_KEY_BackSpace:
				key = MwKEY_BACKSPACE;
				break;
			case XKB_KEY_Left:
				key = MwKEY_LEFT;
				break;
			case XKB_KEY_Right:
				key = MwKEY_RIGHT;
				break;
			case XKB_KEY_Up:
				key = MwKEY_UP;
				break;
			case XKB_KEY_Down:
				key = MwKEY_DOWN;
				break;
			case XKB_KEY_Return:
				key = MwKEY_ENTER;
				break;
			case XKB_KEY_Escape:
				key = MwKEY_ESCAPE;
				break;
			case XKB_KEY_Shift_L:
				key = MwKEY_LEFTSHIFT;
				break;
			case XKB_KEY_Shift_R:
				key = MwKEY_RIGHTSHIFT;
				break;
			case XKB_KEY_Alt_L:
			case XKB_KEY_Alt_R:
				key = MwKEY_ALT;
				break;
			case XKB_KEY_Control_L:
			case XKB_KEY_Control_R:
				key = MwKEY_CONTROL;
				break;
			default:
				if(MwStringIsKeyUTF8(sym)) {
					key = sym;
				}
			}

			if((self->wayland.mod_state & 4) == 4) {
				key |= MwKEY_CONTROL_FLAG;
			}
			if((self->wayland.mod_state & 8) == 8) {
				key |= MwKEY_ALT_FLAG;
			}

			if(key != -1) {
				if(state == WL_KEYBOARD_KEY_STATE_PRESSED) {
					self->wayland.holding_key      = MwTRUE;
					self->wayland.last_pressed_key = key;
				} else {
					self->wayland.holding_key = MwFALSE;
				}
				MwLLRecursiveKeyDispatch(self, &key, state == WL_KEYBOARD_KEY_STATE_PRESSED);
				self->wayland.start_time   = MwTimeGetTick();
				self->wayland.next_elapsed = self->wayland.keyboard_delay;
			}
		}
	}

	if(!MwWaylandVulkan) {
		MwLLDispatch(self, draw, NULL);
	}

	WAYLAND_EVENT_OP_END(self);
};

/* `wl_keyboard.modifiers` callback */
static void keyboard_modifiers(void*		   data,
			       struct wl_keyboard* wl_keyboard,
			       MwU32		   serial,
			       MwU32		   mods_depressed,
			       MwU32		   mods_latched,
			       MwU32		   mods_locked,
			       MwU32		   group) {
	MwLL self = data;
	(void)wl_keyboard;
	(void)serial;
	(void)group;
	(void)mods_latched;

	WAYLAND_EVENT_OP_START(self);

	self->wayland.mod_state = 0;
	self->wayland.mod_state |= mods_depressed;
	self->wayland.mod_state |= mods_locked;

	WAYLAND_EVENT_OP_END(self);
};

static void setup_zwp_clipboard(MwLL self, struct wl_seat* wl_seat) {
	wl_clipboard_device_context_t* device_ctx_zwp = NULL;
	device_ctx_zwp				      = malloc(sizeof(wl_clipboard_device_context_t));
	memset(device_ctx_zwp, 0, sizeof(wl_clipboard_device_context_t));
	device_ctx_zwp->device.zwp = zwp_primary_selection_device_manager_v1_get_device(self->wayland.clipboard_manager.zwp, wl_seat);
	device_ctx_zwp->ll	   = self;
	zwp_primary_selection_device_v1_add_listener(device_ctx_zwp->device.zwp, &zwp_primary_selection_device_v1_listener, device_ctx_zwp);
	arrpush(self->wayland.clipboard_devices_zwp, device_ctx_zwp);

	device_ctx_zwp->accepted_types[0] = "text/plain";
	device_ctx_zwp->accepted_types[1] = "text/plain;charset=utf-8";
	device_ctx_zwp->accepted_types[2] = "TEXT";
	device_ctx_zwp->accepted_types[3] = "STRING";
	device_ctx_zwp->accepted_types[4] = "UTF8_STRING";
	device_ctx_zwp->accepted_types[5] = "text/uri-list";
}
static void setup_clipboard(MwLL self, struct wl_seat* wl_seat) {
	wl_clipboard_device_context_t* device_ctx_wl = malloc(sizeof(wl_clipboard_device_context_t));
	memset(device_ctx_wl, 0, sizeof(wl_clipboard_device_context_t));

	device_ctx_wl->device.wl = wl_data_device_manager_get_data_device(self->wayland.clipboard_manager.wl, wl_seat);
	device_ctx_wl->ll	 = self;

	wl_data_device_add_listener(device_ctx_wl->device.wl, &wl_data_device_listener, device_ctx_wl);
	arrpush(self->wayland.clipboard_devices_wl, device_ctx_wl);

	device_ctx_wl->accepted_types[0] = "text/plain";
	device_ctx_wl->accepted_types[1] = "text/plain;charset=utf-8";
	device_ctx_wl->accepted_types[2] = "TEXT";
	device_ctx_wl->accepted_types[3] = "STRING";
	device_ctx_wl->accepted_types[4] = "UTF8_STRING";
	device_ctx_wl->accepted_types[5] = "text/uri-list";
};

static void keyboard_repeat_info(void*		     data,
				 struct wl_keyboard* wl_keyboard,
				 int32_t	     rate,
				 int32_t	     delay) {
	MwLL self = data;

	(void)wl_keyboard;

	self->wayland.keyboard_rate  = rate;
	self->wayland.keyboard_delay = delay;
};

struct wl_keyboard_listener keyboard_listener = {
    .keymap	 = keyboard_keymap,
    .enter	 = keyboard_enter,
    .leave	 = keyboard_leave,
    .key	 = keyboard_key,
    .modifiers	 = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info,
};

/* `wl_seat.name` callback */
static void
wl_seat_name(void* data, struct wl_seat* wl_seat, const char* name) {
	(void)data;
	(void)wl_seat;
	(void)name;
};

/* `wl_seat.capabilities` callback */
static void wl_seat_capabilities(void* data, struct wl_seat* wl_seat,
				 MwU32 capabilities) {
	MwLL self = data;

	WAYLAND_EVENT_OP_START(self);

	/* Always remember the seat itself, regardless of which capabilities it has - this is
	 * both what setup_clipboard()/setup_zwp_clipboard() below use, and what lets
	 * wl_seat_interface_destroy() release it later even on a keyboard-only seat. */
	self->wayland.pointer_seat = wl_seat;

	if(capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
		self->wayland.keyboard = wl_seat_get_keyboard(wl_seat);
		wl_keyboard_add_listener(self->wayland.keyboard, &keyboard_listener, data);
	}
	if(capabilities & WL_SEAT_CAPABILITY_POINTER) {
		self->wayland.pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(self->wayland.pointer, &pointer_listener, data);
	}
	WAYLAND_EVENT_OP_END(self);

	/* Only hooked up if this seat actually has a pointer: wl_data_device.get_data_device()
	 * requires a non-null seat, and pointer_seat is only ever null before the first
	 * capabilities event has arrived at all. */
	if(capabilities & WL_SEAT_CAPABILITY_POINTER) {
		if(self->wayland.clipboard_manager.wl)
			setup_clipboard(self, self->wayland.pointer_seat);
		if(self->wayland.clipboard_manager.zwp)
			setup_zwp_clipboard(self, self->wayland.pointer_seat);
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
			    int32_t	      transform) {
	(void)data;
	(void)wl_output;
	(void)x;
	(void)y;
	(void)physical_width;
	(void)physical_height;
	(void)subpixel;
	(void)make;
	(void)model;
	(void)transform;
};
static void output_mode(void*		  data,
			struct wl_output* wl_output,
			uint32_t	  flags,
			int32_t		  width,
			int32_t		  height,
			int32_t		  refresh) {
	MwLL self = data;
	(void)wl_output;
	(void)flags;
	(void)refresh;
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
	(void)data;
	xdg_wm_base_pong(xdg_wm_base, serial);
};

/* wl_seat setup function */
static wayland_protocol_t* wl_seat_setup(MwU32 name, MwLL ll, MwU32 version) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	MwU32		    ver	  = (version >= 4) ? 4 : 1; /* version 4 needed for wl_seat_release */
	proto->listener		  = malloc(sizeof(struct wl_seat_listener));
	proto->context		  = NULL;

	((struct wl_seat_listener*)proto->listener)->name	  = wl_seat_name;
	((struct wl_seat_listener*)proto->listener)->capabilities = wl_seat_capabilities;

	wl_seat_add_listener(wl_registry_bind(ll->wayland.registry, name, &wl_seat_interface, ver), proto->listener, ll);

	return proto;
}

static void wl_seat_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	if(wayland->pointer != NULL) {
		if(wl_proxy_get_version((struct wl_proxy*)wayland->pointer) >= WL_POINTER_RELEASE_SINCE_VERSION) {
			wl_pointer_release(wayland->pointer);
		} else {
			wl_pointer_destroy(wayland->pointer);
		}
		wayland->pointer = NULL;
	}
	if(wayland->keyboard != NULL) {
		if(wl_proxy_get_version((struct wl_proxy*)wayland->keyboard) >= WL_KEYBOARD_RELEASE_SINCE_VERSION) {
			wl_keyboard_release(wayland->keyboard);
		} else {
			wl_keyboard_destroy(wayland->keyboard);
		}
		wayland->keyboard = NULL;
	}
	if(wayland->pointer_seat != NULL) {
		if(wl_proxy_get_version((struct wl_proxy*)wayland->pointer_seat) >= WL_SEAT_RELEASE_SINCE_VERSION) {
			wl_seat_release(wayland->pointer_seat);
		} else {
			wl_seat_destroy(wayland->pointer_seat);
		}
		wayland->pointer_seat = NULL;
	}

	free(data->listener);
	free(data);
}

/* wl_output setup function */
static wayland_protocol_t* wl_output_setup(MwU32 name, MwLL ll, MwU32 version) {
	(void)version;
	ll->wayland.output = wl_registry_bind(ll->wayland.registry, name, &wl_output_interface, 1);
	wl_output_add_listener(ll->wayland.output, &output_listener, ll);

	return NULL;
}

static void wl_output_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

/* wl_compositor setup function */
static wayland_protocol_t* wl_compositor_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->compositor = wl_registry_bind(wayland->registry, name, &wl_compositor_interface, 1);

	return NULL;
}

static void wl_compositor_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

/* wl_subcompositor setup function */
static wayland_protocol_t* wl_subcompositor_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	if(wayland->type == MwLL_WAYLAND_TOPLEVEL) {
		wayland->toplevel->scompositor = wl_registry_bind(wayland->registry, name, &wl_subcompositor_interface, 1);
	}

	return NULL;
}

static void wl_subcompositor_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)data;
	if(wayland->type == MwLL_WAYLAND_TOPLEVEL) {
		wl_subcompositor_destroy(wayland->toplevel->scompositor);
	}
}

/* xdg_wm_base setup function */
static wayland_protocol_t* xdg_wm_base_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));
	(void)version;
	proto->listener = malloc(sizeof(struct xdg_wm_base_listener));

	((struct xdg_wm_base_listener*)proto->listener)->ping = xdg_wm_base_ping;

	proto->context = wl_registry_bind(wayland->registry, name, &xdg_wm_base_interface, 1);
	xdg_wm_base_add_listener(proto->context, proto->listener, wayland);

	return proto;
}

static void xdg_wm_base_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	free(data->listener);
	free(data);
}

/* zxdg_decoration_manager_v1 setup function */
static wayland_protocol_t* zxdg_decoration_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	wayland_protocol_t*		      proto = malloc(sizeof(wayland_protocol_t));
	zxdg_decoration_manager_v1_context_t* ctx   = malloc(sizeof(zxdg_decoration_manager_v1_context_t));
	(void)version;
	proto->listener = NULL;

	ctx->manager = wl_registry_bind(wayland->registry, name, &zxdg_decoration_manager_v1_interface, 1);
	;

	proto->context = ctx;

	return proto;
}

static void zxdg_decoration_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

/* wl_shm setup function */
static wayland_protocol_t* wl_shm_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	(void)version;
	wayland->framebuffer.shm = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);
	wayland->backbuffer.shm	 = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);
	wayland->cursor.shm	 = wl_registry_bind(wayland->registry, name, &wl_shm_interface, 1);

	return NULL;
}

static void wl_shm_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	(void)data;
}

static wayland_protocol_t* xdg_toplevel_icon_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));

	(void)version;

	proto->context	= wl_registry_bind(wayland->registry, name, &xdg_toplevel_icon_manager_v1_interface, 1);
	proto->listener = NULL;

	return proto;
}

static void xdg_toplevel_icon_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;
	free(data->listener);
	free(data);
}

static wayland_protocol_t* zwlr_layer_shell_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));

	proto->context	= wl_registry_bind(wayland->registry, name, &zwlr_layer_shell_v1_interface, version);
	proto->listener = NULL;

	return proto;
}

static void zwlr_layer_shell_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;

	free(data);
}

static wayland_protocol_t* wp_fifo_manager_v1_setup(MwU32 name, struct _MwLLWayland* wayland, MwU32 version) {
	wayland_protocol_t* proto = malloc(sizeof(wayland_protocol_t));

	proto->context	= wl_registry_bind(wayland->registry, name, &wp_fifo_manager_v1_interface, version);
	proto->listener = NULL;

	return proto;
}

static void wp_fifo_manager_v1_interface_destroy(struct _MwLLWayland* wayland, wayland_protocol_t* data) {
	(void)wayland;

	free(data);
}

/* Function for setting up the callbacks/structs that will be registered upon the relevant interfaces being found. */
void MwLLWaylandSetupCallbacks(struct _MwLLWayland* wayland) {
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

	if(wayland->wl_protocol_map) {
		shfree(wayland->wl_protocol_map);
	}
	if(wayland->wl_protocol_setup_map) {
		shfree(wayland->wl_protocol_setup_map);
	}

	wayland->wl_protocol_setup_map = NULL;
	wayland->wl_protocol_map       = NULL;

	sh_new_arena(wayland->wl_protocol_map);
	sh_new_arena(wayland->wl_protocol_setup_map);

	WL_INTERFACE(wl_shm);
	WL_INTERFACE(wl_compositor);
	WL_INTERFACE(wl_output);
	WL_INTERFACE(wl_data_device_manager);
	WL_INTERFACE(zwp_primary_selection_device_manager_v1);
	WL_INTERFACE(zwp_pointer_constraints_v1);
	WL_INTERFACE(zwp_relative_pointer_manager_v1);
	WL_INTERFACE(xdg_wm_base);
	WL_INTERFACE(zwlr_layer_shell_v1); /* Only used for layer surface, but we use it always if it's turned into a tool window */
	if(wayland->type == MwLL_WAYLAND_TOPLEVEL) {
		WL_INTERFACE(zxdg_decoration_manager_v1);
		WL_INTERFACE(xdg_toplevel_icon_manager_v1);
		WL_INTERFACE(wl_subcompositor);
		WL_INTERFACE(wl_seat);
		WL_INTERFACE(wp_fifo_manager_v1);
	} else if(wayland->type == MwLL_WAYLAND_POPUP) {
		WL_INTERFACE(wl_seat);
	} else if(wayland->type == MwLL_WAYLAND_LAYER_SURFACE) {
		WL_INTERFACE(wl_subcompositor);
		WL_INTERFACE(wl_seat);
	} else {
		WL_INTERFACE(wl_subcompositor);
	}
#undef WL_INTERFACE
}

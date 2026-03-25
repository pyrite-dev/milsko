/*!
 * @file Mw/LowLevel/Wayland.h
 * @brief Work in progress Wayland Backend
 * @warning This is used internally.
 * @warning This is disabled by default.
 */
#ifndef __MW_LOWLEVEL_WAYLAND_H__
#define __MW_LOWLEVEL_WAYLAND_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>
#include <Mw/Abstract/Dynamic.h>

#include <xkbcommon/xkbcommon.h>
#include <cairo/cairo.h>
#include <pthread.h>

MWDECL int MwLLWaylandCallInit(void);

#define CSD_BORDER_FRAME_LEFT 5
#define CSD_BORDER_FRAME_RIGHT 5
#define CSD_BORDER_FRAME_TOP 24
#define CSD_BORDER_FRAME_BOTTOM 5

struct _MwLLWayland;

#include <wayland-client-core.h>

#if WAYLAND_LOAD_OPENGL
#include <EGL/egl.h>
#include <wayland-egl-core.h>
#endif

typedef struct wayland_call_table {
	void* lib;
	void* xkb_lib;
	void* cairo_lib;
#if WAYLAND_LOAD_OPENGL
#endif

	int (*wl_display_dispatch_pending)(struct wl_display* display);
	void (*wl_display_disconnect)(struct wl_display* display);
	void (*wl_display_cancel_read)(struct wl_display* display);
	int (*wl_display_prepare_read)(struct wl_display* display);
	int (*wl_display_read_events)(struct wl_display* display);
	int (*wl_display_flush)(struct wl_display* display);
	int (*wl_display_get_fd)(struct wl_display* display);
	int (*wl_display_roundtrip)(struct wl_display* display);
	int (*wl_proxy_add_listener)(struct wl_proxy* proxy, void (**implementation)(void), void* data);
	void (*wl_proxy_destroy)(struct wl_proxy* proxy);
	void (*wl_log_set_handler_client)(wl_log_func_t handler);
	struct wl_proxy* (*wl_proxy_marshal_flags)(struct wl_proxy* proxy, uint32_t opcode,
						   const struct wl_interface* interface,
						   uint32_t		      version,
						   uint32_t		      flags, ...);
	struct wl_display* (*wl_display_connect)(const char* name);
	uint32_t (*wl_proxy_get_version)(struct wl_proxy* proxy);

	struct xkb_context* (*xkb_context_new)(enum xkb_context_flags flags);
	void (*xkb_context_unref)(struct xkb_context* context);
	struct xkb_state* (*xkb_state_new)(struct xkb_keymap* keymap);
	void (*xkb_state_unref)(struct xkb_state* state);
	struct xkb_keymap* (*xkb_keymap_new_from_string)(struct xkb_context* context, const char* string, enum xkb_keymap_format format, enum xkb_keymap_compile_flags flags);
	xkb_level_index_t (*xkb_keymap_num_levels_for_key)(struct xkb_keymap* keymap, xkb_keycode_t key, xkb_layout_index_t layout);
	int (*xkb_keymap_key_get_syms_by_level)(struct xkb_keymap* keymap, xkb_keycode_t key, xkb_layout_index_t layout, xkb_level_index_t level, const xkb_keysym_t** syms_out);
	xkb_layout_index_t (*xkb_state_key_get_layout)(struct xkb_state* state, xkb_keycode_t key);
	void (*xkb_keymap_unref)(struct xkb_keymap* keymap);

	void (*cairo_set_line_width)(cairo_t* cr,
				     double   width);
	void (*cairo_scale)(cairo_t* cr,
			    double   sx,
			    double   sy);
	cairo_t* (*cairo_create)(cairo_surface_t* target);
	void (*cairo_move_to)(cairo_t* cr,
			      double   x,
			      double   y);

	void (*cairo_rectangle)(cairo_t* cr,
				double	 x,
				double	 y,
				double	 width,
				double	 height);
	void (*cairo_new_path)(cairo_t* cr);
	void (*cairo_set_line_cap)(cairo_t*	    cr,
				   cairo_line_cap_t line_cap);
	void (*cairo_set_source_rgba)(cairo_t* cr,
				      double   red,
				      double   green,
				      double   blue,
				      double   alpha);
	void (*cairo_set_source_rgb)(cairo_t* cr,
				     double   red,
				     double   green,
				     double   blue);
	void (*cairo_reset_clip)(cairo_t* cr);
	unsigned char* (*cairo_image_surface_get_data)(cairo_surface_t* surface);
	void (*cairo_line_to)(cairo_t* cr,
			      double   x,
			      double   y);
	void (*cairo_surface_destroy)(cairo_surface_t* surface);
	cairo_surface_t* (*cairo_image_surface_create_for_data)(unsigned char* data,
								cairo_format_t format,
								int	       width,
								int	       height,
								int	       stride);
	cairo_surface_t* (*cairo_image_surface_create)(cairo_format_t format,
						       int	      width,
						       int	      height);
	void (*cairo_destroy)(cairo_t* cr);
	cairo_pattern_t* (*cairo_get_source)(cairo_t* cr);
	void (*cairo_close_path)(cairo_t* cr);
	void (*cairo_paint)(cairo_t* cr);
	void (*cairo_surface_flush)(cairo_surface_t* surface);
	void (*cairo_clip)(cairo_t* cr);
	void (*cairo_surface_mark_dirty)(cairo_surface_t* surface);
	void (*cairo_stroke)(cairo_t* cr);
	void (*cairo_set_source_surface)(cairo_t*	  cr,
					 cairo_surface_t* surface,
					 double		  x,
					 double		  y);
	void (*cairo_fill)(cairo_t* cr);
	void (*cairo_pattern_set_filter)(cairo_pattern_t* pattern,
					 cairo_filter_t	  filter);

} wayland_call_table_t;

extern wayland_call_table_t wl_call_tbl;
extern MwBool		    MwWaylandAlwaysRender;
/* defined inline right here so that it doesn't conflict with the other macros */
MwInline int wayland_load_funcs() {
	wl_call_tbl.lib = MwDynamicOpen("libwayland-client.so");
	if(!wl_call_tbl.lib) {
		printf("(libwayland-client.so not found, falling back to X11)\n");
		return 1;
	}
	wl_call_tbl.xkb_lib = MwDynamicOpen("libxkbcommon.so");
	if(!wl_call_tbl.xkb_lib) {
		printf("(libxkbcommon.so not found, cannot use Wayland backend.)\n");
		return 1;
	}
	wl_call_tbl.cairo_lib = MwDynamicOpen("libcairo.so");
	if(!wl_call_tbl.cairo_lib) {
		printf("(libcairo.so not found, cannot use Wayland backend.)\n");
		return 1;
	}
#define WAYLAND_FUNC(x) \
	wl_call_tbl.x = MwDynamicSymbol(wl_call_tbl.lib, #x); \
	if(!wl_call_tbl.x) { \
		printf("WARNING: Could use Wayland backend if " #x " was found.\n"); \
		return 1; \
	};

	WAYLAND_FUNC(wl_display_dispatch_pending)
	WAYLAND_FUNC(wl_display_disconnect)
	WAYLAND_FUNC(wl_display_cancel_read)
	WAYLAND_FUNC(wl_display_prepare_read)
	WAYLAND_FUNC(wl_display_read_events)
	WAYLAND_FUNC(wl_display_flush)
	WAYLAND_FUNC(wl_display_get_fd)
	WAYLAND_FUNC(wl_display_roundtrip)
	WAYLAND_FUNC(wl_proxy_add_listener)
	WAYLAND_FUNC(wl_proxy_destroy)
	WAYLAND_FUNC(wl_log_set_handler_client)
	WAYLAND_FUNC(wl_proxy_marshal_flags)
	WAYLAND_FUNC(wl_display_connect)
	WAYLAND_FUNC(wl_proxy_get_version)

#undef WAYLAND_FUNC

#define XKB_FUNC(x) \
	wl_call_tbl.x = MwDynamicSymbol(wl_call_tbl.xkb_lib, #x); \
	if(!wl_call_tbl.x) { \
		printf("WARNING: Could use Wayland backend if " #x " was found. Do you have libxkbcommon?\n"); \
		return 1; \
	};

	XKB_FUNC(xkb_state_unref)
	XKB_FUNC(xkb_context_new)
	XKB_FUNC(xkb_context_unref)
	XKB_FUNC(xkb_state_new)
	XKB_FUNC(xkb_keymap_new_from_string)
	XKB_FUNC(xkb_keymap_unref)
	XKB_FUNC(xkb_keymap_num_levels_for_key)
	XKB_FUNC(xkb_keymap_key_get_syms_by_level)
	XKB_FUNC(xkb_state_key_get_layout)
#undef XKB_FUNC

#define CAIRO_FUNC(x) \
	wl_call_tbl.x = MwDynamicSymbol(wl_call_tbl.cairo_lib, #x); \
	if(!wl_call_tbl.x) { \
		printf("WARNING: Could use Wayland backend if " #x " was found. Do you have libcairo?\n"); \
		return 1; \
	};

	CAIRO_FUNC(cairo_set_line_width);
	CAIRO_FUNC(cairo_scale);
	CAIRO_FUNC(cairo_create);
	CAIRO_FUNC(cairo_move_to);
	CAIRO_FUNC(cairo_rectangle);
	CAIRO_FUNC(cairo_new_path);
	CAIRO_FUNC(cairo_set_line_cap);
	CAIRO_FUNC(cairo_set_source_rgba);
	CAIRO_FUNC(cairo_set_source_rgb);
	CAIRO_FUNC(cairo_reset_clip);
	CAIRO_FUNC(cairo_image_surface_get_data);
	CAIRO_FUNC(cairo_line_to);
	CAIRO_FUNC(cairo_surface_destroy);
	CAIRO_FUNC(cairo_image_surface_create_for_data);
	CAIRO_FUNC(cairo_image_surface_create);
	CAIRO_FUNC(cairo_destroy);
	CAIRO_FUNC(cairo_get_source);
	CAIRO_FUNC(cairo_close_path);
	CAIRO_FUNC(cairo_paint);
	CAIRO_FUNC(cairo_surface_flush);
	CAIRO_FUNC(cairo_clip);
	CAIRO_FUNC(cairo_surface_mark_dirty);
	CAIRO_FUNC(cairo_stroke);
	CAIRO_FUNC(cairo_set_source_surface);
	CAIRO_FUNC(cairo_fill);
	CAIRO_FUNC(cairo_pattern_set_filter);
#undef CAIRO_FUNC

	return 0;
}

#define wl_display_dispatch_pending wl_call_tbl.wl_display_dispatch_pending
#define wl_display_disconnect wl_call_tbl.wl_display_disconnect
#define wl_display_cancel_read wl_call_tbl.wl_display_cancel_read
#define wl_display_prepare_read wl_call_tbl.wl_display_prepare_read
#define wl_display_read_events wl_call_tbl.wl_display_read_events
#define wl_display_flush wl_call_tbl.wl_display_flush
#define wl_display_get_fd wl_call_tbl.wl_display_get_fd
#define wl_display_roundtrip wl_call_tbl.wl_display_roundtrip
#define wl_proxy_add_listener wl_call_tbl.wl_proxy_add_listener
#define wl_proxy_destroy wl_call_tbl.wl_proxy_destroy
#define wl_log_set_handler_client wl_call_tbl.wl_log_set_handler_client
#define wl_proxy_marshal_flags wl_call_tbl.wl_proxy_marshal_flags
#define wl_display_connect wl_call_tbl.wl_display_connect
#define wl_proxy_get_version wl_call_tbl.wl_proxy_get_version

#define xkb_state_unref wl_call_tbl.xkb_state_unref
#define xkb_context_new wl_call_tbl.xkb_context_new
#define xkb_context_unref wl_call_tbl.xkb_context_unref
#define xkb_state_new wl_call_tbl.xkb_state_new
#define xkb_keymap_new_from_string wl_call_tbl.xkb_keymap_new_from_string
#define xkb_keymap_unref wl_call_tbl.xkb_keymap_unref
#define xkb_keymap_num_levels_for_key wl_call_tbl.xkb_keymap_num_levels_for_key
#define xkb_keymap_key_get_syms_by_level wl_call_tbl.xkb_keymap_key_get_syms_by_level
#define xkb_state_key_get_layout wl_call_tbl.xkb_state_key_get_layout

#define cairo_set_line_width wl_call_tbl.cairo_set_line_width
#define cairo_scale wl_call_tbl.cairo_scale
#define cairo_create wl_call_tbl.cairo_create
#define cairo_move_to wl_call_tbl.cairo_move_to
#define cairo_rectangle wl_call_tbl.cairo_rectangle
#define cairo_new_path wl_call_tbl.cairo_new_path
#define cairo_set_line_cap wl_call_tbl.cairo_set_line_cap
#define cairo_set_source_rgba wl_call_tbl.cairo_set_source_rgba
#define cairo_set_source_rgb wl_call_tbl.cairo_set_source_rgb
#define cairo_reset_clip wl_call_tbl.cairo_reset_clip
#define cairo_image_surface_get_data wl_call_tbl.cairo_image_surface_get_data
#define cairo_line_to wl_call_tbl.cairo_line_to
#define cairo_surface_destroy wl_call_tbl.cairo_surface_destroy
#define cairo_image_surface_create_for_data wl_call_tbl.cairo_image_surface_create_for_data
#define cairo_image_surface_create wl_call_tbl.cairo_image_surface_create
#define cairo_destroy wl_call_tbl.cairo_destroy
#define cairo_get_source wl_call_tbl.cairo_get_source
#define cairo_close_path wl_call_tbl.cairo_close_path
#define cairo_paint wl_call_tbl.cairo_paint
#define cairo_surface_flush wl_call_tbl.cairo_surface_flush
#define cairo_clip wl_call_tbl.cairo_clip
#define cairo_surface_mark_dirty wl_call_tbl.cairo_surface_mark_dirty
#define cairo_stroke wl_call_tbl.cairo_stroke
#define cairo_set_source_surface wl_call_tbl.cairo_set_source_surface
#define cairo_fill wl_call_tbl.cairo_fill
#define cairo_pattern_set_filter wl_call_tbl.cairo_pattern_set_filter

#ifndef WL_PROTOCOLS_DEFINED
#define WL_PROTOCOLS_DEFINED
#include "Wayland/wayland-core-protocol.h"
#include "Wayland/xdg-shell-client-protocol.h"
#include "Wayland/viewporter-client-protocol.h"
#include "Wayland/xdg-decoration-client-protocol.h"
#include "Wayland/cursor-shape-client-protocol.h"
#include "Wayland/primary-selection-client-protocol.h"
#include "Wayland/xdg-toplevel-icon-client-protocol.h"
#endif

typedef struct wayland_protocol {
	void* listener;
	void* context;
} wayland_protocol_t;

typedef wayland_protocol_t*(wl_setup_func)(MwU32, struct _MwLLWayland*);
typedef void(wl_destroy_func)(struct _MwLLWayland* wayland, wayland_protocol_t* data);

typedef struct wayland_protocol_callback_table {
	wl_setup_func*	 setup;
	wl_destroy_func* destroy;
} wayland_protocol_callback_table_t;

struct _MwLLWaylandTopLevel {
	struct xdg_surface*	     xdg_surface;
	struct xdg_toplevel*	     xdg_top_level;
	struct xdg_toplevel_listener xdg_toplevel_listener;
	struct xdg_surface_listener  xdg_surface_listener;

	struct xkb_context* xkb_context;
	struct xkb_keymap*  xkb_keymap;
	struct xkb_state*   xkb_state;

	MwBool compositor_created;
	MwBool xdg_wm_base_created;
	MwBool xdg_surface_created;

	struct wl_subsurface*	 ssurface;
	struct wl_subcompositor* scompositor;
};

struct _MwLLWaylandSublevel {
	struct wl_subsurface*	 subsurface;
	struct wl_subcompositor* subcompositor;

	MwLL parent;

	struct xdg_surface* parent_xdg_surface;
};

struct _MwLLWaylandPopup {
	struct xdg_surface*	    xdg_surface;
	struct xdg_popup*	    xdg_popup;
	struct xdg_positioner*	    xdg_positioner;
	struct xdg_surface_listener xdg_surface_listener;
	struct xdg_wm_base*	    xdg_wm_base;
	MwLL			    topmost_parent;
};

/* Shared set of anything needed for a shm buffer.  */
struct _MwLLWaylandShmBuffer {
	struct wl_shm*	    shm;
	struct wl_shm_pool* shm_pool;
	struct wl_buffer*   shm_buffer;
	struct wl_surface*  surface;
	struct wl_output*   output;

	MwU8*  buf;
	MwU64  buf_size;
	int    fd;
	MwBool setup;
};

enum _MwLLWaylandType {
	MWLL_WAYLAND_UNKNOWN = 0,
	MWLL_WAYLAND_TOPLEVEL,
	MWLL_WAYLAND_SUBLEVEL,
	MWLL_WAYLAND_POPUP,
};

typedef struct wl_clipboard_device_context {
	union {
		struct wl_data_device*			wl;
		struct zwp_primary_selection_device_v1* zwp;
	} device;
	union {
		struct wl_data_offer*		       wl;
		struct zwp_primary_selection_offer_v1* zwp;
	} offer;

	MwLL		ll;
	struct wl_seat* seat;
	MwU32		capabilities;
} wl_clipboard_device_context_t;

struct _MwLLWayland {
	struct _MwLLCommon common;

	union {
		/* Pointer for data that's only loaded if the widget is a toplevel */
		struct _MwLLWaylandTopLevel* toplevel;
		/* Pointer for data that's only loaded if the widget is a sublevel */
		struct _MwLLWaylandSublevel* sublevel;
		/* Pointer for data that's only loaded if the widget is a popup */
		struct _MwLLWaylandPopup* popup;
	};

	enum _MwLLWaylandType type;
	enum _MwLLWaylandType type_to_be;

	MwBool	detatching;
	MwPoint detach_point;

	MwBool did_initial_resize;

	/* Map of Wayland interfaces to their relevant setup functions. */
	struct {
		const char*			   key;
		wayland_protocol_callback_table_t* value;
	}* wl_protocol_setup_map;

	/* Map of Wayland interfaces to any information we keep about them once we've registered them. */
	struct {
		const char*	    key;
		wayland_protocol_t* value;
	}* wl_protocol_map;

	MwBool has_decorations;
	char   title[255];

	struct wl_display*	    display;
	struct wl_registry*	    registry;
	struct wl_compositor*	    compositor;
	struct wl_registry_listener registry_listener;
	struct wl_region*	    o_region;
	struct wl_region*	    region;
	struct wl_output*	    output;

	struct wp_viewport* vp;

	/* clipboard related stuff.
	 * Note that unlike most interfaces, we don't keep zwp_primary_selection stuff in a wayland_protocol_t because we use wl_data_device as a fallback and want to have it share memory space.*/

	union {
		struct wl_data_device_manager*			wl;
		struct zwp_primary_selection_device_manager_v1* zwp;
	} clipboard_manager;
	union {
		struct wl_data_source*			wl;
		struct zwp_primary_selection_source_v1* zwp;
	} clipboard_source;
	char*  clipboard_buffer;
	MwBool supports_zwp;

	uint32_t			clipboard_serial;
	wl_clipboard_device_context_t** clipboard_devices;

	struct wl_pointer* pointer;
	MwU32		   pointer_serial;
	struct wl_seat*	   pointer_seat;

	struct wl_keyboard* keyboard;
	MwU32		    keyboard_serial;

	MwU64 events_pending;

	MwU32 mod_state;

	MwBool configured; /* Whether or not xdg_toplevel_configure has run once */

	MwBool held_down;

	MwI32	x, y;
	MwI32	ox, oy;
	MwU32	ww, wh;	       /* Window position */
	MwPoint cur_mouse_pos; /* Currently known mouse position */

	MwU32 mw, mh; /* Monitor width and height as advertised by wl_output.mode */

	MwLL parent;

	MwBool force_render;
	MwBool did_event_loop_early;

	struct _MwLLWaylandShmBuffer  framebuffer;
	struct _MwLLWaylandShmBuffer  backbuffer;
	struct _MwLLWaylandShmBuffer  cursor;
	struct _MwLLWaylandShmBuffer* icon;

	/*
	Events mutex. Any time a keyboard/mouse event happens, we try to lock this for 100 milliseconds, then give up if we can't do it. This is used in conjunction with some code in the MwLLDestroyImpl to make sure that destroy is NEVER interferes with an ongoing event.

	IOI_XD: This sounds like a hilariously rare edge case, so it's almost funnier that this happened with 100% certainty for me and I spent day(s) trying to figure out what was happening. */
	pthread_mutex_t eventsMutex;

	uint32_t last_time;

	MwBool moving;

	cairo_surface_t* front_cs;
	cairo_surface_t* back_cs;
	cairo_t*	 front_cairo;
	cairo_t*	 back_cairo;
	/* The cairo to actually use for draw operations. Typically is front_cairo, but MwLLBeginDraw can change this to the back_cairo so it can be used to draw window decorations. */
	cairo_t* selected_cairo;
};

struct _MwLLWaylandColor {
	struct _MwLLCommonColor common;
};

struct _MwLLWaylandPixmap {
	struct _MwLLCommonPixmap common;

	cairo_surface_t* cs;
};

#endif

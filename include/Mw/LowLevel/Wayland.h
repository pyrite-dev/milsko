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

#include <wayland-client-protocol.h>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include <cairo/cairo.h>

MWDECL int MwLLWaylandCallInit(void);

#ifndef WL_PROTOCOLS_DEFINED
#define WL_PROTOCOLS_DEFINED
#include "Wayland/xdg-shell-client-protocol.h"
#include "Wayland/xdg-decoration-client-protocol.h"
#include "Wayland/cursor-shape-client-protocol.h"
#include "Wayland/primary-selection-client-protocol.h"
#include "Wayland/xdg-toplevel-icon-client-protocol.h"
#endif

struct _MwLLWayland;

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
	MwLL			    parent;
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

	MwBool always_render;

	struct wl_display*	    display;
	struct wl_registry*	    registry;
	struct wl_compositor*	    compositor;
	struct wl_registry_listener registry_listener;
	struct wl_region*	    region;
	struct wl_output*	    output;

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

	struct wl_pointer*  pointer;
	MwU32		    pointer_serial;
	struct wl_keyboard* keyboard;
	MwU32		    keyboard_serial;

	MwU64 events_pending;

	MwU32 mod_state;

	MwBool configured; /* Whether or not xdg_toplevel_configure has run once */

	MwU32	x, y, ww, wh;  /* Window position */
	MwPoint cur_mouse_pos; /* Currently known mouse position */

	MwU32 mw, mh; /* Monitor width and height as advertised by wl_output.mode */

	MwLL parent;

	MwBool force_render;

	MwBool disabled;

	struct _MwLLWaylandShmBuffer  framebuffer;
	struct _MwLLWaylandShmBuffer  cursor;
	struct _MwLLWaylandShmBuffer* icon;

	cairo_surface_t* cs;
	cairo_t*	 cairo;
};

struct _MwLLWaylandColor {
	struct _MwLLCommonColor common;
};

struct _MwLLWaylandPixmap {
	struct _MwLLCommonPixmap common;

	cairo_surface_t* cs;
};

#endif

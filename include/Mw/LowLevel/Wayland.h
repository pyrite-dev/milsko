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

	struct wl_display*	    display;
	struct wl_registry*	    registry;
	struct wl_compositor*	    compositor;
	struct wl_registry_listener registry_listener;
	struct wl_region*	    region;
	struct wl_output*	    output;

	struct wl_pointer*  pointer;
	struct wl_keyboard* keyboard;
	MwU32		    pointer_serial;

	MwBool events_pending;
	MwBool test;

	MwU32 mod_state;

	MwBool configured; /* Whether or not xdg_toplevel_configure has run once */

	MwU32	x, y, ww, wh;  /* Window position */
	MwPoint cur_mouse_pos; /* Currently known mouse position */

	MwU32 mw, mh; /* Monitor width and height as advertised by wl_output.mode */

	MwLL parent;

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

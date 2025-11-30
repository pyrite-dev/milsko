/* $Id$ */
/*!
 * @file Mw/LowLevel/Wayland.h
 * @brief Wayland Backend
 * @warning This is used internally
 */
#ifndef __MW_LOWLEVEL_WAYLAND_H__
#define __MW_LOWLEVEL_WAYLAND_H__

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/LowLevel.h>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

MWDECL int MwLLWaylandCallInit(void);

#ifndef WL_PROTOCOLS_DEFINED
#define WL_PROTOCOLS_DEFINED
#include "Wayland/xdg-shell-client-protocol.h"
#include "Wayland/xdg-decoration-client-protocol.h"
#include "Wayland/cursor-shape-client-protocol.h"
#endif

typedef struct wayland_protocol {
	void* listener;
	void* context;
} wayland_protocol_t;

typedef wayland_protocol_t*(wl_setup_func)(MwU32, struct _MwLLWayland*);

struct _MwLLWayland {
	struct _MwLLCommon common;

	// struct wl_shm*	      shm;
	struct wl_display*  display;
	struct wl_registry* registry;
	// struct wl_compositor* compositor;
	// struct wl_seat*	      seat;
	struct wl_surface*  surface;
	struct xdg_surface* xdg_surface;
	// struct xdg_wm_base*   xdg_wm_base;
	struct xdg_toplevel* xdg_top_level;

	struct wl_registry_listener registry_listener;

	// struct zxdg_decoration_manager_v1*  decoration_manager;
	// struct zxdg_toplevel_decoration_v1* toplevel_decoration;

	struct xdg_toplevel_listener xdg_toplevel_listener;
	// struct xdg_wm_base_listener  xdg_wm_base_listener;
	struct xdg_surface_listener xdg_surface_listener;
	// struct wl_pointer_listener		    pointer_listener;
	// struct wl_seat_listener			    seat_listener;
	// struct wl_keyboard_listener		    keyboard_listener;
	// struct wl_shell_surface_listener	    shell_surface_listener;
	// struct zxdg_toplevel_decoration_v1_listener decoration_listener;

	struct {
		const char*    key;
		wl_setup_func* value;
	}* wl_protocol_setup_map;

	struct {
		const char*	    key;
		wayland_protocol_t* value;
	}* wl_protocol_map;

	struct xkb_context* xkb_context;

	MwBool configured;
	MwBool running;
};

struct _MwLLWaylandColor {
	struct _MwLLCommonColor common;
};

struct _MwLLWaylandPixmap {
	struct _MwLLCommonPixmap common;
};

#endif

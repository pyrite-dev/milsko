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

struct _MwLLWaylandTopLevel {
	struct xdg_surface*	     xdg_surface;
	struct xdg_toplevel*	     xdg_top_level;
	struct xdg_toplevel_listener xdg_toplevel_listener;
	struct xdg_surface_listener  xdg_surface_listener;

	struct xkb_context* xkb_context;

	MwBool running;
	MwBool compositor_created;
	MwBool xdg_wm_base_created;
	MwBool xdg_surface_created;
};

struct _MwLLWayland {
	struct _MwLLCommon common;
	union {
		struct _MwLLWaylandTopLevel* toplevel;
		struct wl_subsurface*	     subsurface;
	};
	enum {
		MWLL_WAYLAND_TOPLEVEL	= 0,
		MWLL_WAYLAND_SUBSURFACE = 1,
	} type;

	struct wl_display*	    display;
	struct wl_registry*	    registry;
	struct wl_compositor*	    compositor;
	struct wl_subcompositor*    subcompositor;
	struct wl_surface*	    surface;
	struct wl_shm*		    shm;
	struct wl_registry_listener registry_listener;

	struct {
		const char*    key;
		wl_setup_func* value;
	}* wl_protocol_setup_map;

	struct {
		const char*	    key;
		wayland_protocol_t* value;
	}* wl_protocol_map;

	MwBool configured;
	MwU32  x, y, ww, wh;
};

struct _MwLLWaylandColor {
	struct _MwLLCommonColor common;
};

struct _MwLLWaylandPixmap {
	struct _MwLLCommonPixmap common;
};

#endif

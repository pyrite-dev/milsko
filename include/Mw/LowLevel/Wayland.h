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
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

MWDECL int MwLLWaylandCallInit(void);

#ifndef WL_PROTOCOLS_DEFINED
#define WL_PROTOCOLS_DEFINED
#include "Wayland/xdg-shell-client-protocol.h"
#include "Wayland/xdg-decoration-client-protocol.h"
#include "Wayland/cursor-shape-client-protocol.h"
#endif

struct _MwLLWayland;

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
	struct xkb_keymap*  xkb_keymap;
	struct xkb_state*   xkb_state;

	MwBool compositor_created;
	MwBool xdg_wm_base_created;
	MwBool xdg_surface_created;
};

struct _MwLLWayland {
	struct _MwLLCommon common;

	/* Pointer for data that's only loaded if the widget is a toplevel */
	struct _MwLLWaylandTopLevel* toplevel;

	enum {
		MWLL_WAYLAND_TOPLEVEL = 0,
		MWLL_WAYLAND_SUBLEVEL = 1, /* Sublevels are surfaces that have the toplevel as a parent. They could be implemented as subsurfaces if we ever switch away from OpenGL. Some parts of the code also call them subwidgets. */
	} type;

	/* Map of Wayland interfaces to their relevant setup functions. */
	struct {
		const char*    key;
		wl_setup_func* value;
	}* wl_protocol_setup_map;

	/* Map of Wayland interfaces to any information we keep about them once we've registered them. */
	struct {
		const char*	    key;
		wayland_protocol_t* value;
	}* wl_protocol_map;

	struct wl_display*	    display;
	struct wl_registry*	    registry;
	struct wl_compositor*	    compositor;
	struct wl_subcompositor*    subcompositor;
	struct wl_surface*	    surface;
	struct wl_registry_listener registry_listener;
	struct wl_event_queue*	    event_queue;

	EGLNativeWindowType egl_window_native;
	EGLDisplay	    egl_display;
	EGLContext	    egl_context;
	EGLSurface	    egl_surface;
	EGLConfig	    egl_config;

	MwLL*  sublevels;  /* stb_ds managed array of any sublevels */
	MwBool configured; /* Whether or not xdg_toplevel_configure has run once */
	MwBool egl_setup;  /* Whether or not EGL has been set up */
	MwBool has_set_xy /* Whether or not MwSetXY has been called */;

	int	resize_counter; /* Counter that's for a hack in event_loop */
	MwU32	x, y, ww, wh;	/* Window position */
	MwU32	lw, lh;		/* Last known window position */
	MwPoint cur_mouse_pos;	/* Currently known mouse position */

	struct timeval timer;
	MwU64	       cooldown_timer;
	MwU64	       cooldown_timer_epoch;

	MwLL parent;
	MwLL topmost_parent; /* The parent at the top of all the other parents. Usually a toplevel. */
};

struct _MwLLWaylandColor {
	struct _MwLLCommonColor common;
};

struct _MwLLWaylandPixmap {
	struct _MwLLCommonPixmap common;
	GLuint			 texture;
	MwBool			 texture_deleted;
};

#endif

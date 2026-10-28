#include <Mw/Milsko.h>

#include <QDOffscreen.h>
#include <Menus.h>
#include <Devices.h>
#include <Scrap.h>
#include <LowMem.h>
#include <ToolUtils.h>
#include <Sound.h>
#include <AppleEvents.h>
#include <Drag.h>
#include <Gestalt.h>
#include <Icons.h>
#include <Processes.h>

#include "../../external/stb_ds.h"

/* Classic Mac OS has no child windows. Every toplevel handle owns a WindowPtr and an
 * offscreen GWorld holding the whole content area; child handles are rectangles inside it.
 * Drawing for any handle goes into the GWorld clipped to the part of the handle that is not
 * covered by its children or by siblings above it, then gets copied to the window.
 * Update events only have to copy the GWorld back to the screen. */

#define BITS(port) (&((GrafPtr)(port))->portBits)

enum qevent_type {
	QEVENT_DRAW = 0,
	QEVENT_RESIZE,
	QEVENT_CLIPBOARD,
	QEVENT_DROP
};

typedef struct qevent {
	int   type;
	MwLL  handle;
	char* data;
} qevent_t;

enum menu_id {
	MENU_APPLE = 128,
	MENU_FILE
};

static MwLL*	 handles = NULL;
static qevent_t* queue	 = NULL;

static MwLL capture = NULL;
static MwLL focus   = NULL;
static MwLL hover   = NULL;

static Point last_mouse;

/* pointer grab: motion is measured against where the pointer was warped to, or against where it was
 * before the warp, since the warp does not stick where pointer positions are absolute (an emulator in a window) */
static MwLL  grab = NULL;
static Point grab_ref;
static Point grab_alt;
static short last_modifiers = 0;

static MwLL	 cur_draw = NULL;
static CGrafPtr	 saved_port;
static GDHandle	 saved_device;
static RgnHandle vis_rgn;
static RgnHandle tmp_rgn;

static PixMapHandle dummy_pixmap;

static void append_ditl(Handle h, Size* len, short top, short left, short bottom, short right, unsigned char type, const unsigned char* text) {
	Size  n = 4 + 8 + 2 + text[0] + (text[0] & 1);
	char* p;

	SetHandleSize(h, *len + n);
	if(MemError() != noErr) return;

	p = *h + *len;
	memset(p, 0, n);
	((short*)(p + 4))[0] = top;
	((short*)(p + 4))[1] = left;
	((short*)(p + 4))[2] = bottom;
	((short*)(p + 4))[3] = right;
	p[12]		     = type;
	p[13]		     = text[0];
	memcpy(p + 14, text + 1, text[0]);

	(**(short**)h)++;
	*len += n;
}

static void init_toolbox(void) {
	static int done = 0;

	if(done) return;
	done = 1;

	MaxApplZone();
	MoreMasters();
	MoreMasters();

	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();
}

/* there is no console to print to. the item list is built in memory, so this works without any
 * resources in the application. like MessageBox on Windows, this works before MwLibraryInit */
void MwLLQuickdrawAlert(const char* text) {
	Handle	  items = NewHandleClear(2);
	Size	  len	= 2;
	Rect	  rc, screen = qd.screenBits.bounds;
	DialogPtr dlg;
	short	  item = 0;
	GrafPtr	  old;
	Str255	  msg;
	char*	  s = MwUTF8TextToACPText(text);
	size_t	  l = strlen(s);
	size_t	  i;

	init_toolbox();

	if(l > 255) l = 255;
	msg[0] = l;
	memcpy(msg + 1, s, l);
	free(s);

	for(i = 1; i <= l; i++) {
		if(msg[i] == '\n') msg[i] = '\r';
	}

	if(items == NULL) {
		SysBeep(1);
		return;
	}

	/* the count is stored minus one */
	**(short**)items = -1;
	append_ditl(items, &len, 110, 290, 130, 360, ctrlItem + btnCtrl, "\pOK");
	append_ditl(items, &len, 10, 10, 100, 360, statText + itemDisable, msg);

	SetRect(&rc, 0, 0, 370, 140);
	OffsetRect(&rc, (screen.right - screen.left - 370) / 2, (screen.bottom - screen.top - 140) / 3);

	GetPort(&old);
	if((dlg = NewDialog(NULL, &rc, "\p", true, dBoxProc, (WindowPtr)-1L, false, 0, items)) == NULL) {
		DisposeHandle(items);
		SysBeep(1);
		return;
	}

	SetDialogDefaultItem(dlg, 1);
	InitCursor();
	while(item != 1) ModalDialog(NULL, &item);

	DisposeDialog(dlg);
	SetPort(old);
}

static void out_of_memory(void) {
	MwLLQuickdrawAlert("Out of memory. Try giving the application a larger memory partition in its Get Info window.");
}

static int is_alive(MwLL handle) {
	int i;

	if(handle == NULL) return 0;
	for(i = 0; i < arrlen(handles); i++) {
		if(handles[i] == handle) return 1;
	}

	return 0;
}

static void push_event(int type, MwLL handle) {
	qevent_t e;
	int	 i;

	for(i = 0; i < arrlen(queue); i++) {
		if(queue[i].type == type && queue[i].handle == handle) return;
	}

	e.type	 = type;
	e.handle = handle;
	e.data	 = NULL;
	arrput(queue, e);
}

static void push_draw_tree(MwLL handle) {
	int i;

	push_event(QEVENT_DRAW, handle);
	for(i = 0; i < arrlen(handle->quickdraw.children); i++) {
		push_draw_tree(handle->quickdraw.children[i]);
	}
}

/* the area handle covers right now is about to be uncovered: redraw the parent and
 * every sibling (and their children) overlapping it, like WM_PAINT would on Windows */
static void expose(MwLL handle) {
	MwLL p = handle->quickdraw.parent;
	int  i;

	if(p == NULL || !handle->quickdraw.shown) return;

	push_event(QEVENT_DRAW, p);
	for(i = 0; i < arrlen(p->quickdraw.children); i++) {
		MwLL s = p->quickdraw.children[i];

		if(s == handle || !s->quickdraw.shown) continue;
		if(s->quickdraw.x >= handle->quickdraw.x + handle->quickdraw.width || handle->quickdraw.x >= s->quickdraw.x + s->quickdraw.width) continue;
		if(s->quickdraw.y >= handle->quickdraw.y + handle->quickdraw.height || handle->quickdraw.y >= s->quickdraw.y + s->quickdraw.height) continue;

		push_draw_tree(s);
	}
}

static void remove_events(MwLL handle) {
	int i;

	for(i = arrlen(queue) - 1; i >= 0; i--) {
		if(queue[i].handle == handle) {
			free(queue[i].data);
			arrdel(queue, i);
		}
	}
}

static MwLL get_top(MwLL handle) {
	while(handle->quickdraw.parent != NULL) handle = handle->quickdraw.parent;

	return handle->quickdraw.window == NULL ? NULL : handle;
}

/* offset of handle inside of its toplevel window */
static void get_offset(MwLL handle, int* x, int* y) {
	*x = *y = 0;
	while(handle->quickdraw.parent != NULL) {
		*x += handle->quickdraw.x;
		*y += handle->quickdraw.y;

		handle = handle->quickdraw.parent;
	}
}

static void window_origin(WindowPtr window, Point* p) {
	GrafPtr old;

	GetPort(&old);
	SetPort(window);
	p->h = p->v = 0;
	LocalToGlobal(p);
	SetPort(old);
}

static void rect_rgn(RgnHandle rgn, int x, int y, int w, int h) {
	SetRectRgn(rgn, x, y, x + w, y + h);
}

/* region of the toplevel window where handle is visible */
static void calc_vis(MwLL handle, RgnHandle rgn) {
	MwLL n = handle;
	int  nx, ny, i;

	if(get_top(handle) == NULL) {
		SetEmptyRgn(rgn);
		return;
	}

	get_offset(handle, &nx, &ny);
	rect_rgn(rgn, nx, ny, handle->quickdraw.width, handle->quickdraw.height);

	if(handle->quickdraw.has_clip) {
		MwRect* c = &handle->quickdraw.clip;

		rect_rgn(tmp_rgn, nx + c->x, ny + c->y, c->width, c->height);
		SectRgn(rgn, tmp_rgn, rgn);
	}

	for(i = 0; i < arrlen(handle->quickdraw.children); i++) {
		MwLL c = handle->quickdraw.children[i];
		if(!c->quickdraw.shown) continue;

		rect_rgn(tmp_rgn, nx + c->quickdraw.x, ny + c->quickdraw.y, c->quickdraw.width, c->quickdraw.height);
		DiffRgn(rgn, tmp_rgn, rgn);
	}

	while(n->quickdraw.parent != NULL) {
		MwLL p	= n->quickdraw.parent;
		int  px = nx - n->quickdraw.x;
		int  py = ny - n->quickdraw.y;
		int  after;

		if(!n->quickdraw.shown) {
			SetEmptyRgn(rgn);
			return;
		}

		rect_rgn(tmp_rgn, px, py, p->quickdraw.width, p->quickdraw.height);
		SectRgn(rgn, tmp_rgn, rgn);

		after = 0;
		for(i = 0; i < arrlen(p->quickdraw.children); i++) {
			MwLL s = p->quickdraw.children[i];

			if(s == n) {
				after = 1;
			} else if(after && s->quickdraw.shown) {
				rect_rgn(tmp_rgn, px + s->quickdraw.x, py + s->quickdraw.y, s->quickdraw.width, s->quickdraw.height);
				DiffRgn(rgn, tmp_rgn, rgn);
			}
		}

		n  = p;
		nx = px;
		ny = py;
	}
}

static void apply_clip(MwLL handle) {
	int x, y;

	get_offset(handle, &x, &y);
	calc_vis(handle, vis_rgn);
	CopyRgn(vis_rgn, tmp_rgn);
	OffsetRgn(tmp_rgn, -x, -y);
	SetClip(tmp_rgn);
}

static void leave_draw(void) {
	CGrafPtr port;
	GDHandle device;

	if(cur_draw == NULL) return;

	GetGWorld(&port, &device);
	SetOrigin(0, 0);
	ClipRect(&port->portRect);
	SetGWorld(saved_port, saved_device);

	cur_draw = NULL;
}

/* make the GWorld of the handle's toplevel the current port, with the handle's origin and clip */
static int enter_draw(MwLL handle) {
	MwLL top;
	int  x, y;

	if(cur_draw == handle) return 1;
	leave_draw();

	if((top = get_top(handle)) == NULL || top->quickdraw.gworld == NULL) return 0;

	GetGWorld(&saved_port, &saved_device);
	SetGWorld(top->quickdraw.gworld, NULL);

	get_offset(handle, &x, &y);
	SetOrigin(-x, -y);
	apply_clip(handle);

	cur_draw = handle;

	return 1;
}

/* copy what handle is responsible for from the GWorld to the window */
static void flush_draw(MwLL handle) {
	MwLL	top;
	GrafPtr old;

	/* something else (OpenGL) draws directly into the window there */
	if(handle->common.copy_buffer == 0) return;
	if((top = get_top(handle)) == NULL || top->quickdraw.gworld == NULL) return;

	calc_vis(handle, vis_rgn);
	if(EmptyRgn(vis_rgn)) return;

	GetPort(&old);
	SetPort(top->quickdraw.window);
	ForeColor(blackColor);
	BackColor(whiteColor);
	CopyBits(BITS(top->quickdraw.gworld), BITS(top->quickdraw.window), &(**vis_rgn).rgnBBox, &(**vis_rgn).rgnBBox, srcCopy, vis_rgn);
	SetPort(old);
}

#define PRIM_BEGIN(handle) \
	if(!enter_draw(handle)) return;
#define PRIM_END(handle) \
	if(!handle->quickdraw.drawing) { \
		leave_draw(); \
		flush_draw(handle); \
	}

/* removes the areas of handles that draw directly into the window (OpenGL) from rgn */
static void subtract_direct(MwLL handle, RgnHandle rgn, int x, int y) {
	int i;

	for(i = 0; i < arrlen(handle->quickdraw.children); i++) {
		MwLL c = handle->quickdraw.children[i];
		if(!c->quickdraw.shown) continue;

		if(c->common.copy_buffer == 0) {
			rect_rgn(tmp_rgn, x + c->quickdraw.x, y + c->quickdraw.y, c->quickdraw.width, c->quickdraw.height);
			DiffRgn(rgn, tmp_rgn, rgn);
		} else {
			subtract_direct(c, rgn, x + c->quickdraw.x, y + c->quickdraw.y);
		}
	}
}

static void blit_window(MwLL top) {
	GrafPtr old;

	if(top->quickdraw.gworld == NULL) return;

	RectRgn(vis_rgn, &top->quickdraw.window->portRect);
	subtract_direct(top, vis_rgn, 0, 0);

	GetPort(&old);
	SetPort(top->quickdraw.window);
	ForeColor(blackColor);
	BackColor(whiteColor);
	CopyBits(BITS(top->quickdraw.gworld), BITS(top->quickdraw.window), &top->quickdraw.window->portRect, &top->quickdraw.window->portRect, srcCopy, vis_rgn);
	SetPort(old);
}

static void create_gworld(MwLL handle) {
	Rect	 rc;
	CGrafPtr port;
	GDHandle device;

	if(cur_draw != NULL && get_top(cur_draw) == handle) leave_draw();

	if(handle->quickdraw.gworld != NULL) DisposeGWorld(handle->quickdraw.gworld);
	handle->quickdraw.gworld = NULL;

	SetRect(&rc, 0, 0, handle->quickdraw.width > 0 ? handle->quickdraw.width : 1, handle->quickdraw.height > 0 ? handle->quickdraw.height : 1);

	/* prefer temporary memory so the application partition does not need to fit the framebuffer */
	if(NewGWorld(&handle->quickdraw.gworld, 0, &rc, NULL, NULL, useTempMem) != noErr) {
		if(NewGWorld(&handle->quickdraw.gworld, 0, &rc, NULL, NULL, 0) != noErr) {
			out_of_memory();
			handle->quickdraw.gworld = NULL;
			return;
		}
	}

	LockPixels(GetGWorldPixMap(handle->quickdraw.gworld));

	GetGWorld(&port, &device);
	SetGWorld(handle->quickdraw.gworld, NULL);
	BackColor(whiteColor);
	EraseRect(&rc);
	SetGWorld(port, device);
}

static short window_proc(MwLL handle) {
	if(handle->quickdraw.tool || handle->quickdraw.borderless) return plainDBox;
	if(handle->quickdraw.min_w > 0 && handle->quickdraw.min_w == handle->quickdraw.max_w && handle->quickdraw.min_h == handle->quickdraw.max_h) return noGrowDocProc;

	return zoomDocProc;
}

/* keep the title bar of normal windows reachable, menus and dropdowns go exactly where asked */
static void clamp_position(MwLL handle, int* x, int* y) {
	Rect screen = qd.screenBits.bounds;
	int  top    = GetMBarHeight() + 20;

	if(handle->quickdraw.tool || handle->quickdraw.borderless) return;

	if(*x + handle->quickdraw.width > screen.right) *x = screen.right - handle->quickdraw.width;
	if(*x < screen.left + 2) *x = screen.left + 2;
	if(*y + handle->quickdraw.height > screen.bottom) *y = screen.bottom - handle->quickdraw.height;
	if(*y < top) *y = top;
}

static void raise_tools(void) {
	int i;

	for(i = 0; i < arrlen(handles); i++) {
		MwLL h = handles[i];
		if(h->quickdraw.window != NULL && h->quickdraw.tool) BringToFront(h->quickdraw.window);
	}
}

static MwLL from_window(WindowPtr window) {
	MwLL h;

	if(window == NULL) return NULL;

	h = (MwLL)GetWRefCon(window);
	if(!is_alive(h) || h->quickdraw.window != window) return NULL;

	return h;
}

/* window icons are proxy icons (Mac OS 8.5 and later), made with Icon Services. both get loaded at runtime */
static struct icontbl {
	int   state; /* 0 not loaded yet, 1 available, -1 not available */
	void* windowslib;
	void* iconlib;

	OSStatus (*SetWindowProxyIcon)(WindowRef window, IconRef icon);
	OSStatus (*SetWindowModified)(WindowRef window, Boolean modified);
	OSErr (*SetIconFamilyData)(IconFamilyHandle family, OSType type, Handle data);
	OSErr (*RegisterIconRefFromIconFamily)(OSType creator, OSType type, IconFamilyHandle family, IconRef* icon);
	OSErr (*UnregisterIconRef)(OSType creator, OSType type);
	OSErr (*ReleaseIconRef)(IconRef icon);
} itbl;

static int icon_setup(void) {
	if(itbl.state != 0) return itbl.state == 1;
	itbl.state = -1;

	if((itbl.windowslib = MwDynamicOpen("WindowsLib")) == NULL || (itbl.iconlib = MwDynamicOpen("IconServicesLib")) == NULL) return 0;

	if((itbl.SetWindowProxyIcon = MwDynamicSymbol(itbl.windowslib, "SetWindowProxyIcon")) == NULL) return 0;
	if((itbl.SetWindowModified = MwDynamicSymbol(itbl.windowslib, "SetWindowModified")) == NULL) return 0;
	if((itbl.SetIconFamilyData = MwDynamicSymbol(itbl.iconlib, "SetIconFamilyData")) == NULL) return 0;
	if((itbl.RegisterIconRefFromIconFamily = MwDynamicSymbol(itbl.iconlib, "RegisterIconRefFromIconFamily")) == NULL) return 0;
	if((itbl.UnregisterIconRef = MwDynamicSymbol(itbl.iconlib, "UnregisterIconRef")) == NULL) return 0;
	if((itbl.ReleaseIconRef = MwDynamicSymbol(itbl.iconlib, "ReleaseIconRef")) == NULL) return 0;

	itbl.state = 1;
	return 1;
}

static void free_icon(MwLL handle) {
	if(handle->quickdraw.icon == NULL) return;

	itbl.ReleaseIconRef(handle->quickdraw.icon);
	itbl.UnregisterIconRef(handle->quickdraw.icon_creator, handle->quickdraw.icon_type);
	handle->quickdraw.icon = NULL;
}

/* 32-bit icon data is packed per channel; this only uses literal runs, of up to 128 bytes */
static void pack_channel(unsigned char* out, int* n, const unsigned char* rgba, int channel) {
	int i, j;

	for(i = 0; i < 256; i += 128) {
		out[(*n)++] = 127;
		for(j = i; j < i + 128; j++) out[(*n)++] = rgba[j * 4 + channel];
	}
}

/* pixmap scaled to 16x16, averaging the source pixels each icon pixel covers */
static void scale_icon(MwLLPixmap pixmap, unsigned char* rgba) {
	int x, y;

	for(y = 0; y < 16; y++) {
		for(x = 0; x < 16; x++) {
			int	      x0 = x * pixmap->common.width / 16, x1 = (x + 1) * pixmap->common.width / 16;
			int	      y0 = y * pixmap->common.height / 16, y1 = (y + 1) * pixmap->common.height / 16;
			unsigned long sum[4] = {0, 0, 0, 0}, count = 0;
			int	      sx, sy, c;

			if(x1 <= x0) x1 = x0 + 1;
			if(y1 <= y0) y1 = y0 + 1;
			for(sy = y0; sy < y1; sy++) {
				for(sx = x0; sx < x1; sx++) {
					unsigned char* px = &pixmap->common.raw[(sy * pixmap->common.width + sx) * 4];

					for(c = 0; c < 4; c++) sum[c] += px[c];
					count++;
				}
			}
			for(c = 0; c < 4; c++) rgba[(y * 16 + x) * 4 + c] = sum[c] / count;
		}
	}
}

/* 1-bit icon and mask, for 1-bit screens: a black silhouette */
static void silhouette(const unsigned char* rgba, unsigned char* bw) {
	int i;

	memset(bw, 0, 64);
	for(i = 0; i < 256; i++) {
		if(rgba[i * 4 + 3] >= 128) {
			bw[i / 8] |= 0x80 >> (i % 8);
			bw[32 + i / 8] |= 0x80 >> (i % 8);
		}
	}
}

/* icon family for a 16x16 icon */
static IconFamilyHandle make_icon_family(const unsigned char* rgba) {
	unsigned char	 packed[3 * 258];
	unsigned char	 bw[64];
	IconFamilyHandle family;
	Handle		 h;
	int		 x, n = 0;

	if((family = (IconFamilyHandle)NewHandle(sizeof(IconFamilyResource) - sizeof(IconFamilyElement))) == NULL) return NULL;
	(**family).resourceType = kIconFamilyType;
	(**family).resourceSize = sizeof(IconFamilyResource) - sizeof(IconFamilyElement);

	pack_channel(packed, &n, rgba, 0);
	pack_channel(packed, &n, rgba, 1);
	pack_channel(packed, &n, rgba, 2);
	if(PtrToHand(packed, &h, n) == noErr) {
		itbl.SetIconFamilyData(family, kSmall32BitData, h);
		DisposeHandle(h);
	}

	for(x = 0; x < 256; x++) packed[x] = rgba[x * 4 + 3];
	if(PtrToHand(packed, &h, 256) == noErr) {
		itbl.SetIconFamilyData(family, kSmall8BitMask, h);
		DisposeHandle(h);
	}

	silhouette(rgba, bw);
	if(PtrToHand(bw, &h, sizeof(bw)) == noErr) {
		itbl.SetIconFamilyData(family, kSmall1BitMask, h);
		DisposeHandle(h);
	}

	return family;
}

/* the Apple menu can show the icon of the application as its title: since System 7, a title of the marker byte 1
 * followed by an IconSuiteRef is drawn as that icon */
static IconSuiteRef apple_suite = NULL;
static MwLL	    apple_owner = NULL;

static void set_apple_title(const unsigned char* title) {
	MenuHandle old = GetMenuHandle(MENU_APPLE);
	MenuHandle m   = NewMenu(MENU_APPLE, title);

	/* titles cannot be changed in place without Carbon, so the menu gets rebuilt */
	AppendResMenu(m, 'DRVR');
	if(old != NULL) {
		DeleteMenu(MENU_APPLE);
		DisposeMenu(old);
	}
	InsertMenu(m, GetMenuHandle(MENU_FILE) != NULL ? MENU_FILE : 0);

	DrawMenuBar();
}

static void reset_apple_menu(void) {
	if(apple_suite == NULL) return;

	set_apple_title("\p\024");
	DisposeIconSuite(apple_suite, true);
	apple_suite = NULL;
	apple_owner = NULL;
}

/* small icon suite for a 16x16 icon: 1-bit, and 8-bit with the standard system palette */
static IconSuiteRef make_icon_suite(const unsigned char* rgba) {
	unsigned char bw[64];
	unsigned char idx[256];
	CTabHandle    clut;
	IconSuiteRef  suite;
	Handle	      h;
	int	      i, j;

	if(NewIconSuite(&suite) != noErr) return NULL;

	silhouette(rgba, bw);
	if(PtrToHand(bw, &h, sizeof(bw)) == noErr) AddIconToSuite(h, suite, 'ics#');

	if((clut = GetCTable(8)) != NULL) {
		for(i = 0; i < 256; i++) {
			long best = 0x7fffffff;

			idx[i] = 0;
			for(j = 0; j <= (**clut).ctSize; j++) {
				RGBColor* c  = &(**clut).ctTable[j].rgb;
				long	  dr = (c->red >> 8) - rgba[i * 4], dg = (c->green >> 8) - rgba[i * 4 + 1], db = (c->blue >> 8) - rgba[i * 4 + 2];
				long	  d  = dr * dr + dg * dg + db * db;

				if(d < best) {
					best   = d;
					idx[i] = j;
				}
			}
		}
		DisposeCTable(clut);

		if(PtrToHand(idx, &h, sizeof(idx)) == noErr) AddIconToSuite(h, suite, 'ics8');
	}

	return suite;
}

static void set_apple_icon(MwLL handle, const unsigned char* rgba) {
	IconSuiteRef  suite = make_icon_suite(rgba);
	unsigned char title[6];

	if(suite == NULL) return;

	title[0] = 5;
	title[1] = 1;
	memcpy(title + 2, &suite, sizeof(suite));
	set_apple_title(title);

	/* the menu does not own the suite */
	if(apple_suite != NULL) DisposeIconSuite(apple_suite, true);
	apple_suite = suite;
	apple_owner = handle;
}

static void apply_icon(MwLL handle) {
	if(handle->quickdraw.icon == NULL || handle->quickdraw.window == NULL) return;

	itbl.SetWindowProxyIcon(handle->quickdraw.window, handle->quickdraw.icon);

	/* a modified window gets a dimmed proxy icon */
	itbl.SetWindowModified(handle->quickdraw.window, false);
}

/* nothing may keep using the port of a disposed window, QuickDraw would draw into freed memory */
static void dispose_window(WindowPtr window, WindowPtr replacement) {
	GrafPtr port;

	if(replacement == NULL) GetWMgrPort(&replacement);

	GetPort(&port);
	if(port == (GrafPtr)window) SetPort(replacement);
	if(saved_port == (CGrafPtr)window) saved_port = (CGrafPtr)replacement;

	DisposeWindow(window);
}

/* (re)create the window of a toplevel, at global position x/y */
static void create_window(MwLL handle, int x, int y) {
	static long serial = 0;
	Rect	    rc;
	short	    proc  = window_proc(handle);
	int	    shown = handle->quickdraw.shown;
	WindowPtr   old	  = handle->quickdraw.window;

	SetRect(&rc, x, y, x + handle->quickdraw.width, y + handle->quickdraw.height);

	handle->quickdraw.window = NewCWindow(NULL, &rc, handle->quickdraw.title, shown, proc, (WindowPtr)-1L, proc != plainDBox, (long)handle);

	/* the new window can get the address of the old one, this tells them apart (for OpenGL) */
	handle->quickdraw.window_serial = ++serial;

	if(old != NULL) dispose_window(old, handle->quickdraw.window);

	apply_icon(handle);

	/* tool windows (menus, dropdowns) should not steal the highlight from the window they belong to */
	if(handle->quickdraw.tool) {
		WindowPtr w;

		HiliteWindow(handle->quickdraw.window, false);
		for(w = FrontWindow(); w != NULL; w = (WindowPtr)((WindowPeek)w)->nextWindow) {
			MwLL h = from_window(w);

			if(((WindowPeek)w)->visible && (h == NULL || !h->quickdraw.tool)) {
				HiliteWindow(w, true);
				break;
			}
		}
	}
}

/* deepest visible handle containing x/y, which are relative to handle; x/y get translated */
static MwLL hit_test(MwLL handle, int* x, int* y) {
	int i;

	for(i = arrlen(handle->quickdraw.children) - 1; i >= 0; i--) {
		MwLL c = handle->quickdraw.children[i];
		if(!c->quickdraw.shown) continue;

		if(*x >= c->quickdraw.x && *y >= c->quickdraw.y && *x < c->quickdraw.x + c->quickdraw.width && *y < c->quickdraw.y + c->quickdraw.height) {
			*x -= c->quickdraw.x;
			*y -= c->quickdraw.y;
			return hit_test(c, x, y);
		}
	}

	return handle;
}

static void global_to_handle(MwLL handle, Point where, MwPoint* out) {
	MwLL  top = get_top(handle);
	Point o;
	int   x, y;

	if(top == NULL) {
		out->x = out->y = 0;
		return;
	}

	window_origin(top->quickdraw.window, &o);
	get_offset(handle, &x, &y);

	out->x = where.h - o.h - x;
	out->y = where.v - o.v - y;
}

/* handle under the global point, if it is one of ours */
static MwLL handle_at(Point where) {
	WindowPtr window;
	MwLL	  top;
	Point	  o;
	int	  x, y;

	if(FindWindow(where, &window) != inContent || (top = from_window(window)) == NULL) return NULL;

	window_origin(window, &o);
	x = where.h - o.h;
	y = where.v - o.v;

	return hit_test(top, &x, &y);
}

static void set_focus(MwLL handle) {
	if(focus == handle) return;

	if(is_alive(focus)) MwLLDispatch(focus, focus_out, NULL);
	focus = handle;
	if(focus != NULL) MwLLDispatch(focus, focus_in, NULL);
}

static void update_cursor(Point where) {
	MwLL h = grab != NULL ? grab : (capture != NULL ? capture : handle_at(where));

	if(h != NULL && h->quickdraw.has_cursor) {
		SetCursor(h->quickdraw.do_drag_cursor ? &h->quickdraw.cursor_drag : &h->quickdraw.cursor);
	} else {
		SetCursor(&qd.arrow);
	}
	hover = h;
}

static void after_resize(MwLL top) {
	top->quickdraw.width  = top->quickdraw.window->portRect.right - top->quickdraw.window->portRect.left;
	top->quickdraw.height = top->quickdraw.window->portRect.bottom - top->quickdraw.window->portRect.top;

	create_gworld(top);

	push_event(QEVENT_RESIZE, top);
	push_draw_tree(top);
}

static void quit_all(void) {
	int i;

	for(i = 0; i < arrlen(handles); i++) {
		MwLL h = handles[i];
		if(h->quickdraw.window != NULL && !h->quickdraw.tool) {
			MwLLDispatch(h, close, NULL);
			return;
		}
	}
}

/* the required apple events. quit is what the Finder sends at shut down and restart */
static OSErr ae_open_application(const AppleEvent* ev, AppleEvent* reply, long refcon) {
	(void)ev;
	(void)reply;
	(void)refcon;

	return noErr;
}

static OSErr ae_documents(const AppleEvent* ev, AppleEvent* reply, long refcon) {
	(void)ev;
	(void)reply;
	(void)refcon;

	return errAEEventNotHandled;
}

static OSErr ae_quit_application(const AppleEvent* ev, AppleEvent* reply, long refcon) {
	(void)ev;
	(void)reply;
	(void)refcon;

	quit_all();

	return noErr;
}

static void do_menu(long result) {
	short menu = HiWord(result);
	short item = LoWord(result);

	if(menu == MENU_APPLE) {
		Str255 name;

		GetMenuItemText(GetMenuHandle(MENU_APPLE), item, name);
		OpenDeskAcc(name);
	} else if(menu == MENU_FILE && item == 1) {
		quit_all();
	}

	HiliteMenu(0);
}

static int translate_key(EventRecord* ev) {
	int ch = ev->message & charCodeMask;
	int n;

	switch((ev->message & keyCodeMask) >> 8) {
	case 0x7b:
		return MwKEY_LEFT;
	case 0x7c:
		return MwKEY_RIGHT;
	case 0x7e:
		return MwKEY_UP;
	case 0x7d:
		return MwKEY_DOWN;
	case 0x24:
	case 0x4c:
		return MwKEY_ENTER;
	case 0x33:
		return MwKEY_BACKSPACE;
	case 0x35:
		return MwKEY_ESCAPE;
	}

	n = MwMacRomanToUTF32(ch);
	if(n > 0 && n < 0x20) {
		n = ('a' + n - 1) | MwKEY_CONTROL_FLAG;
	} else if(ev->modifiers & cmdKey) {
		/* command is what control is everywhere else */
		if(n >= 'A' && n <= 'Z') n += 'a' - 'A';
		n |= MwKEY_CONTROL_FLAG;
	}

	return n;
}

static MwLL key_target(void) {
	if(is_alive(focus)) return focus;

	return from_window(FrontWindow());
}

static void dispatch_modifiers(short modifiers) {
	static const struct {
		short mask;
		int   key;
	} mods[] = {{shiftKey, MwKEY_LEFTSHIFT}, {rightShiftKey, MwKEY_RIGHTSHIFT}, {controlKey, MwKEY_CONTROL}, {optionKey, MwKEY_ALT}};
	MwLL t	 = key_target();
	int  i;

	for(i = 0; i < (int)(sizeof(mods) / sizeof(mods[0])); i++) {
		int n = mods[i].key;

		if(!((modifiers ^ last_modifiers) & mods[i].mask)) continue;
		if(t == NULL) continue;

		if(modifiers & mods[i].mask) {
			MwLLDispatch(t, key, &n);
		} else {
			MwLLDispatch(t, key_released, &n);
		}
	}

	last_modifiers = modifiers;
}

static void warp_mouse(Point p) {
	LMSetMouseTemp(p);
	LMSetRawMouseLocation(p);
	LMSetCursorNew(true);
}

static void grab_center(MwLL handle, Point* p) {
	Point o;
	int   x, y;

	window_origin(get_top(handle)->quickdraw.window, &o);
	get_offset(handle, &x, &y);

	p->h = o.h + x + handle->quickdraw.width / 2;
	p->v = o.v + y + handle->quickdraw.height / 2;
}

static void dispatch_move(Point where) {
	MwLL	t = grab != NULL ? grab : (capture != NULL ? capture : handle_at(where));
	MwPoint p;

	update_cursor(where);

	if(t == NULL) return;

	if(t == grab && get_top(t) != NULL) {
		Point c, ref;
		long  dr, da;

		/* this is the warp itself */
		if(where.h == grab_ref.h && where.v == grab_ref.v) return;

		dr  = labs(where.h - grab_ref.h) + labs(where.v - grab_ref.v);
		da  = labs(where.h - grab_alt.h) + labs(where.v - grab_alt.v);
		ref = da < dr ? grab_alt : grab_ref;

		p.x = where.h - ref.h;
		p.y = where.v - ref.v;
		if(p.x == 0 && p.y == 0) return;

		MwLLDispatch(t, move, &p);

		grab_center(t, &c);
		grab_alt = where;
		grab_ref = c;
		warp_mouse(c);
	} else {
		global_to_handle(t, where, &p);
		MwLLDispatch(t, move, &p);
	}
}

static void poll_input(EventRecord* ev) {
	if(ev->modifiers != last_modifiers) dispatch_modifiers(ev->modifiers);

	if(ev->where.h != last_mouse.h || ev->where.v != last_mouse.v) {
		last_mouse = ev->where;
		dispatch_move(ev->where);
	}
}

static void handle_mouse_down(EventRecord* ev) {
	WindowPtr window;
	short	  part = FindWindow(ev->where, &window);
	MwLL	  top  = from_window(window);

	switch(part) {
	case inMenuBar:
		do_menu(MenuSelect(ev->where));
		break;
	case inSysWindow:
		SystemClick(ev, window);
		break;
	case inProxyIcon:
	case inDrag:
	{
		Rect bounds = (**GetGrayRgn()).rgnBBox;

		DragWindow(window, ev->where, &bounds);
		raise_tools();
		break;
	}
	case inGrow:
	{
		Rect limit;
		long r;

		if(top == NULL) break;

		SetRect(&limit,
			top->quickdraw.min_w > 0 ? top->quickdraw.min_w : 32,
			top->quickdraw.min_h > 0 ? top->quickdraw.min_h : 32,
			top->quickdraw.max_w > 0 ? top->quickdraw.max_w + 1 : 32767,
			top->quickdraw.max_h > 0 ? top->quickdraw.max_h + 1 : 32767);

		if((r = GrowWindow(window, ev->where, &limit)) != 0) {
			SizeWindow(window, LoWord(r), HiWord(r), true);
			after_resize(top);
		}
		break;
	}
	case inZoomIn:
	case inZoomOut:
		if(top != NULL && TrackBox(window, ev->where, part)) {
			GrafPtr old;

			GetPort(&old);
			SetPort(window);
			EraseRect(&window->portRect);
			ZoomWindow(window, part, false);
			SetPort(old);

			after_resize(top);
		}
		break;
	case inGoAway:
		if(top != NULL && TrackGoAway(window, ev->where)) MwLLDispatch(top, close, NULL);
		break;
	case inContent:
	{
		MwLL	t;
		MwMouse m;

		if(top == NULL) break;

		if(!top->quickdraw.tool && !((WindowPeek)window)->hilited) {
			SelectWindow(window);
			raise_tools();
		}

		if((t = handle_at(ev->where)) == NULL) break;

		global_to_handle(t, ev->where, &m.point);
		m.button = (ev->modifiers & controlKey) ? MwMOUSE_RIGHT : MwMOUSE_LEFT;

		capture = t;
		set_focus(t);
		MwLLDispatch(t, down, &m);
		break;
	}
	}
}

static void handle_mouse_up(EventRecord* ev) {
	MwLL	t = capture != NULL ? capture : handle_at(ev->where);
	MwMouse m;

	capture = NULL;
	if(t == NULL) return;

	global_to_handle(t, ev->where, &m.point);
	m.button = (ev->modifiers & controlKey) ? MwMOUSE_RIGHT : MwMOUSE_LEFT;

	MwLLDispatch(t, up, &m);
}

static void handle_key(EventRecord* ev) {
	MwLL t;
	int  n;

	if(ev->what == keyDown && (ev->modifiers & cmdKey)) {
		long r = MenuKey(ev->message & charCodeMask);

		if(HiWord(r) != 0) {
			do_menu(r);
			return;
		}
	}

	if((t = key_target()) == NULL) return;

	n = translate_key(ev);
	if(ev->what == keyUp) {
		MwLLDispatch(t, key_released, &n);
	} else {
		MwLLDispatch(t, key, &n);
	}
}

static void handle_update(EventRecord* ev) {
	WindowPtr window = (WindowPtr)ev->message;
	MwLL	  top	 = from_window(window);

	BeginUpdate(window);
	if(top != NULL) blit_window(top);
	EndUpdate(window);
}

static void handle_clipboard(MwLL handle) {
	Handle h = NewHandle(0);
	long   offset;
	long   len;

	if(h == NULL) return;

	if((len = GetScrap(h, 'TEXT', &offset)) > 0) {
		char* raw = malloc(len + 1);
		char* text;
		long  i;

		HLock(h);
		memcpy(raw, *h, len);
		HUnlock(h);
		raw[len] = 0;

		for(i = 0; i < len; i++) {
			if(raw[i] == '\r') raw[i] = '\n';
		}

		text = MwACPTextToUTF8Text(raw);
		MwLLDispatch(handle, clipboard, text);

		free(text);
		free(raw);
	}

	DisposeHandle(h);
}

/* dynloaded DragLib */
static struct dragtbl {
	void* lib;

	OSErr (*InstallTrackingHandler)(DragTrackingHandlerUPP handler, WindowPtr window, void* refcon);
	OSErr (*InstallReceiveHandler)(DragReceiveHandlerUPP handler, WindowPtr window, void* refcon);
	OSErr (*CountDragItems)(DragReference drag, UInt16* items);
	OSErr (*GetDragItemReferenceNumber)(DragReference drag, UInt16 index, ItemReference* item);
	OSErr (*GetFlavorFlags)(DragReference drag, ItemReference item, FlavorType type, FlavorFlags* flags);
	OSErr (*GetFlavorData)(DragReference drag, ItemReference item, FlavorType type, void* data, Size* size, UInt32 offset);
	OSErr (*GetDragMouse)(DragReference drag, Point* mouse, Point* pinned);
	OSErr (*ShowDragHilite)(DragReference drag, RgnHandle rgn, Boolean inside);
	OSErr (*HideDragHilite)(DragReference drag);
} dtbl;

static MwLL	 drag_target = NULL;
static RgnHandle drag_rgn;

/* the handle taking a drop at the global point: the accepting handle under it, or its nearest accepting ancestor */
static MwLL drop_target(Point where) {
	MwLL h = handle_at(where);

	while(h != NULL && !h->quickdraw.accepts_dnd) h = h->quickdraw.parent;

	return h;
}

static int drag_has_files(DragReference drag) {
	UInt16	      n, i;
	ItemReference item;
	FlavorFlags   flags;

	if(dtbl.CountDragItems(drag, &n) != noErr) return 0;
	for(i = 1; i <= n; i++) {
		if(dtbl.GetDragItemReferenceNumber(drag, i, &item) == noErr && dtbl.GetFlavorFlags(drag, item, flavorTypeHFS, &flags) == noErr) return 1;
	}

	return 0;
}

/* "Volume:Folder:File", in UTF-8 */
static char* spec_path(FSSpec* spec) {
	char  path[1024];
	char  name[64];
	long  dir = spec->parID;
	int   len;
	char* out;

	memcpy(path, spec->name + 1, spec->name[0]);
	path[spec->name[0]] = 0;

	while(dir != fsRtParID) {
		CInfoPBRec pb;
		Str63	   n;

		memset(&pb, 0, sizeof(pb));
		pb.dirInfo.ioNamePtr   = n;
		pb.dirInfo.ioVRefNum   = spec->vRefNum;
		pb.dirInfo.ioDrDirID   = dir;
		pb.dirInfo.ioFDirIndex = -1;
		if(PBGetCatInfoSync(&pb) != noErr) return NULL;

		memcpy(name, n + 1, n[0]);
		name[n[0]] = 0;

		if(strlen(name) + 1 + strlen(path) + 1 > sizeof(path)) return NULL;
		memmove(path + strlen(name) + 1, path, strlen(path) + 1);
		memcpy(path, name, strlen(name));
		path[strlen(name)] = ':';

		dir = pb.dirInfo.ioDrParID;
	}

	/* a volume itself is "Volume:" */
	if(spec->parID == fsRtParID && (len = strlen(path)) + 2 <= (int)sizeof(path)) {
		path[len]     = ':';
		path[len + 1] = 0;
	}

	out = MwACPTextToUTF8Text(path);

	return out;
}

static OSErr drag_tracking(DragTrackingMessage message, WindowPtr window, void* refcon, DragReference drag) {
	MwLL	t = NULL;
	GrafPtr old;

	(void)refcon;

	if(message == kDragTrackingEnterWindow || message == kDragTrackingInWindow) {
		Point mouse, pin;

		dtbl.GetDragMouse(drag, &mouse, &pin);
		if(drag_has_files(drag) && (t = drop_target(mouse)) != NULL && get_top(t)->quickdraw.window != window) t = NULL;
		if(t) {
			t->quickdraw.do_drag_cursor = MwTRUE;
		}
		update_cursor(mouse);
	}

	if(t != drag_target) {
		GetPort(&old);
		SetPort(window);

		if(drag_target != NULL) dtbl.HideDragHilite(drag);
		if(t != NULL) {
			/* window coordinates are local coordinates of the window */
			calc_vis(t, drag_rgn);
			dtbl.ShowDragHilite(drag, drag_rgn, true);
		}

		SetPort(old);
		drag_target = t;
	}

	return noErr;
}

static OSErr drag_receive(WindowPtr window, void* refcon, DragReference drag) {
	Point  mouse, pin;
	MwLL   t;
	UInt16 n, i;
	int    accepted = 0;

	(void)window;
	(void)refcon;

	if(drag_target != NULL) {
		dtbl.HideDragHilite(drag);
		drag_target = NULL;
	}

	dtbl.GetDragMouse(drag, &mouse, &pin);
	if((t = drop_target(mouse)) == NULL || dtbl.CountDragItems(drag, &n) != noErr) return dragNotAcceptedErr;

	for(i = 1; i <= n; i++) {
		ItemReference item;
		HFSFlavor     hfs;
		Size	      size = sizeof(hfs);
		qevent_t      e;

		if(dtbl.GetDragItemReferenceNumber(drag, i, &item) != noErr) continue;
		if(dtbl.GetFlavorData(drag, item, flavorTypeHFS, &hfs, &size, 0) != noErr) continue;
		if((e.data = spec_path(&hfs.fileSpec)) == NULL) continue;

		/* dispatched from MwLLNextEvent, this runs in the middle of the drag */
		e.type	 = QEVENT_DROP;
		e.handle = t;
		arrput(queue, e);

		accepted = 1;
	}

	t->quickdraw.do_drag_cursor = MwFALSE;
	/* force a redraw */
	update_cursor(mouse);

	return accepted ? noErr : dragNotAcceptedErr;
}

/* the core drains events per handle (MwStep turns property events off for the handle while it
 * does), so a handle should only be given events meant for it. NULL means anyone may take it */
static MwLL event_target(EventRecord* ev) {
	WindowPtr window;

	switch(ev->what) {
	case mouseDown:
		if(FindWindow(ev->where, &window) != inContent) return NULL;
		return handle_at(ev->where);
	case mouseUp:
		return capture != NULL ? capture : handle_at(ev->where);
	case keyDown:
	case keyUp:
	case autoKey:
		if(ev->what == keyDown && (ev->modifiers & cmdKey)) return NULL; /* might be a menu shortcut */
		return key_target();
	}

	return NULL;
}

static int is_mine(MwLL target, MwLL handle, EventRecord* ev) {
	static EventRecord stalled;
	static long	   stalled_since;

	if(target == NULL || target == handle || !is_alive(target)) return 1;

	/* never let an event nobody collects block the queue */
	if(ev->what != stalled.what || ev->when != stalled.when || ev->message != stalled.message || ev->where.h != stalled.where.h || ev->where.v != stalled.where.v) {
		stalled	      = *ev;
		stalled_since = TickCount();
		return 0;
	}

	return TickCount() - stalled_since > 6;
}

static int find_queued(MwLL handle) {
	int i;

	for(i = 0; i < arrlen(queue); i++) {
		if(queue[i].handle == handle) return i;
	}

	return -1;
}

static void restyle(MwLL handle) {
	Point o;

	if(handle->quickdraw.window == NULL || GetWVariant(handle->quickdraw.window) == window_proc(handle)) return;

	window_origin(handle->quickdraw.window, &o);
	create_window(handle, o.h, o.v);
}

/* Mac OS Roman, 0x80 - 0xff */
static const unsigned short macroman[128] = {
    0x00c4, 0x00c5, 0x00c7, 0x00c9, 0x00d1, 0x00d6, 0x00dc, 0x00e1, 0x00e0, 0x00e2, 0x00e4, 0x00e3, 0x00e5, 0x00e7, 0x00e9, 0x00e8,
    0x00ea, 0x00eb, 0x00ed, 0x00ec, 0x00ee, 0x00ef, 0x00f1, 0x00f3, 0x00f2, 0x00f4, 0x00f6, 0x00f5, 0x00fa, 0x00f9, 0x00fb, 0x00fc,
    0x2020, 0x00b0, 0x00a2, 0x00a3, 0x00a7, 0x2022, 0x00b6, 0x00df, 0x00ae, 0x00a9, 0x2122, 0x00b4, 0x00a8, 0x2260, 0x00c6, 0x00d8,
    0x221e, 0x00b1, 0x2264, 0x2265, 0x00a5, 0x00b5, 0x2202, 0x2211, 0x220f, 0x03c0, 0x222b, 0x00aa, 0x00ba, 0x03a9, 0x00e6, 0x00f8,
    0x00bf, 0x00a1, 0x00ac, 0x221a, 0x0192, 0x2248, 0x2206, 0x00ab, 0x00bb, 0x2026, 0x00a0, 0x00c0, 0x00c3, 0x00d5, 0x0152, 0x0153,
    0x2013, 0x2014, 0x201c, 0x201d, 0x2018, 0x2019, 0x00f7, 0x25ca, 0x00ff, 0x0178, 0x2044, 0x20ac, 0x2039, 0x203a, 0xfb01, 0xfb02,
    0x2021, 0x00b7, 0x201a, 0x201e, 0x2030, 0x00c2, 0x00ca, 0x00c1, 0x00cb, 0x00c8, 0x00cd, 0x00ce, 0x00cf, 0x00cc, 0x00d3, 0x00d4,
    0xf8ff, 0x00d2, 0x00da, 0x00db, 0x00d9, 0x0131, 0x02c6, 0x02dc, 0x00af, 0x02d8, 0x02d9, 0x02da, 0x00b8, 0x02dd, 0x02db, 0x02c7};

int MwMacRomanToUTF32(unsigned char c) {
	if(c < 0x80) return c;
	return macroman[c - 0x80];
}

int MwUTF32ToMacRoman(int cp) {
	int i;

	if(cp < 0x80) return cp;
	for(i = 0; i < 128; i++) {
		if(macroman[i] == cp) return 0x80 + i;
	}

	return '?';
}

static MwLL MwLLCreateImpl(MwLL parent, int x, int y, int width, int height) {
	MwLL r = malloc(sizeof(*r));

	if(!r) {
		out_of_memory();
		return NULL;
	}

	memset(r, 0, sizeof(*r));
	MwLLCreateCommon(r);

	r->common.copy_buffer	  = 1;
	r->common.type		  = MwLLBackendQuickDraw;
	r->common.coordinate_type = MwCOORDINATE_GLOBAL;

	r->quickdraw.parent   = parent;
	r->quickdraw.children = NULL;
	r->quickdraw.width    = width > 0 ? width : 1;
	r->quickdraw.height   = height > 0 ? height : 1;
	r->quickdraw.shown    = 1;

	arrput(handles, r);

	if(parent == NULL) {
		Rect screen = qd.screenBits.bounds;

		if(x == MwDEFAULT) x = (screen.right - screen.left - width) / 2;
		if(y == MwDEFAULT) y = (screen.bottom - screen.top - height) / 2;
		clamp_position(r, &x, &y);

		r->quickdraw.title[0] = 0;

		create_window(r, x, y);
		create_gworld(r);

		SelectWindow(r->quickdraw.window);
		raise_tools();
	} else {
		r->quickdraw.x = x == MwDEFAULT ? 0 : x;
		r->quickdraw.y = y == MwDEFAULT ? 0 : y;

		arrput(parent->quickdraw.children, r);
	}

	push_event(QEVENT_DRAW, r);

	return r;
}

static void MwLLDestroyImpl(MwLL handle) {
	int  i;
	MwLL parent = handle->quickdraw.parent;

	if(cur_draw != NULL && (cur_draw == handle || get_top(cur_draw) == get_top(handle))) leave_draw();

	MwLLDestroyCommon(handle);

	remove_events(handle);

	for(i = 0; i < arrlen(handles); i++) {
		if(handles[i] == handle) {
			arrdel(handles, i);
			break;
		}
	}

	if(parent != NULL) {
		expose(handle);

		for(i = 0; i < arrlen(parent->quickdraw.children); i++) {
			if(parent->quickdraw.children[i] == handle) {
				arrdel(parent->quickdraw.children, i);
				break;
			}
		}
	}

	/* children are normally destroyed before their parent, but do not leave them dangling */
	for(i = 0; i < arrlen(handle->quickdraw.children); i++) {
		handle->quickdraw.children[i]->quickdraw.parent = NULL;
	}
	arrfree(handle->quickdraw.children);

	if(capture == handle) capture = NULL;
	if(focus == handle) focus = NULL;
	if(hover == handle) hover = NULL;
	if(grab == handle) grab = NULL;
	if(drag_target == handle) drag_target = NULL;

	if(handle->quickdraw.gworld != NULL) DisposeGWorld(handle->quickdraw.gworld);
	if(handle->quickdraw.window != NULL) dispose_window(handle->quickdraw.window, NULL);
	free_icon(handle);
	if(apple_owner == handle) reset_apple_menu();

	free(handle);
}

static void MwLLBeginDrawImpl(MwLL handle) {
	handle->quickdraw.drawing = 1;
	enter_draw(handle);
}

static void MwLLEndDrawImpl(MwLL handle) {
	if(cur_draw == handle) leave_draw();
	handle->quickdraw.drawing = 0;

	flush_draw(handle);
}

static void MwLLPolygonImpl(MwLL handle, MwPoint* points, int points_count, MwLLColor color) {
	PolyHandle p;
	int	   i;

	if(points_count < 2) return;

	PRIM_BEGIN(handle);

	p = OpenPoly();
	MoveTo(points[0].x, points[0].y);
	for(i = 1; i < points_count; i++) {
		LineTo(points[i].x, points[i].y);
	}
	LineTo(points[0].x, points[0].y);
	ClosePoly();

	PenNormal();
	RGBForeColor(&color->quickdraw.rgb);
	PaintPoly(p);

	KillPoly(p);

	PRIM_END(handle);
}

static void MwLLLineImpl(MwLL handle, MwPoint* points, MwLLColor color) {
	PRIM_BEGIN(handle);

	PenNormal();
	RGBForeColor(&color->quickdraw.rgb);
	MoveTo(points[0].x, points[0].y);
	LineTo(points[1].x, points[1].y);

	PRIM_END(handle);
}

static MwLLColor MwLLAllocColorImpl(MwLL handle, int r, int g, int b) {
	MwLLColor c = malloc(sizeof(*c));

	(void)handle;

	if(!c) {
		out_of_memory();
		return NULL;
	}

	MwLLColorUpdate(c, r, g, b);

	return c;
}

static void MwLLColorUpdateImpl(MwLLColor c, int r, int g, int b) {
	if(r > 255) r = 255;
	if(g > 255) g = 255;
	if(b > 255) b = 255;
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	c->common.red	= r;
	c->common.green = g;
	c->common.blue	= b;

	c->quickdraw.rgb.red   = r * 0x101;
	c->quickdraw.rgb.green = g * 0x101;
	c->quickdraw.rgb.blue  = b * 0x101;
}

static void MwLLFreeColorImpl(MwLLColor color) {
	free(color);
}

static void MwLLGetXYWHImpl(MwLL handle, int* x, int* y, unsigned int* w, unsigned int* h) {
	if(handle->quickdraw.window != NULL) {
		Point o;

		window_origin(handle->quickdraw.window, &o);

		*x = o.h;
		*y = o.v;
	} else {
		*x = handle->quickdraw.x;
		*y = handle->quickdraw.y;
	}

	*w = handle->quickdraw.width;
	*h = handle->quickdraw.height;
}

static void MwLLSetXYImpl(MwLL handle, int x, int y) {
	if(handle->quickdraw.window != NULL) {
		clamp_position(handle, &x, &y);
		MoveWindow(handle->quickdraw.window, x, y, false);
		return;
	}

	if(handle->quickdraw.x == x && handle->quickdraw.y == y) return;

	expose(handle);

	handle->quickdraw.x = x;
	handle->quickdraw.y = y;

	push_draw_tree(handle);
}

static void MwLLSetWHImpl(MwLL handle, int w, int h) {
	if(w < 1) w = 1;
	if(h < 1) h = 1;

	if(handle->quickdraw.window != NULL) {
		SizeWindow(handle->quickdraw.window, w, h, true);
		after_resize(handle);
		return;
	}

	if(handle->quickdraw.width == w && handle->quickdraw.height == h) return;

	expose(handle);

	handle->quickdraw.width	 = w;
	handle->quickdraw.height = h;

	push_event(QEVENT_RESIZE, handle);
	push_draw_tree(handle);
}

static void MwLLSetTitleImpl(MwLL handle, const char* title) {
	char*  s = MwUTF8TextToACPText(title);
	size_t l = strlen(s);

	if(l > 255) l = 255;
	handle->quickdraw.title[0] = l;
	memcpy(handle->quickdraw.title + 1, s, l);

	free(s);

	if(handle->quickdraw.window != NULL) SetWTitle(handle->quickdraw.window, handle->quickdraw.title);
}

static int MwLLPendingImpl(MwLL handle) {
	EventRecord ev;

	if(find_queued(handle) >= 0) return 1;
	if(EventAvail(everyEvent, &ev)) return is_mine(event_target(&ev), handle, &ev);

	/* there are no mouse moved events without the mouse region of WaitNextEvent, so poll */
	if(ev.where.h != last_mouse.h || ev.where.v != last_mouse.v) {
		ev.what = nullEvent;
		return is_mine(grab != NULL ? grab : (capture != NULL ? capture : handle_at(ev.where)), handle, &ev);
	}
	if(ev.modifiers != last_modifiers) {
		ev.what = nullEvent;
		return is_mine(key_target(), handle, &ev);
	}

	return 0;
}

static void MwLLNextEventImpl(MwLL handle) {
	EventRecord ev;
	int	    i;

	if((i = find_queued(handle)) >= 0) {
		qevent_t e = queue[i];

		arrdel(queue, i);

		switch(e.type) {
		case QEVENT_DRAW:
			MwLLDispatch(e.handle, draw, NULL);
			break;
		case QEVENT_RESIZE:
			MwLLDispatch(e.handle, resize, NULL);
			break;
		case QEVENT_CLIPBOARD:
			handle_clipboard(e.handle);
			break;
		case QEVENT_DROP:
			MwLLDispatch(e.handle, drag_and_drop, e.data);
			free(e.data);
			break;
		}

		return;
	}

	if(WaitNextEvent(everyEvent, &ev, 0, NULL)) {
		switch(ev.what) {
		case mouseDown:
			handle_mouse_down(&ev);
			break;
		case mouseUp:
			handle_mouse_up(&ev);
			break;
		case keyDown:
		case keyUp:
		case autoKey:
			handle_key(&ev);
			break;
		case updateEvt:
			handle_update(&ev);
			break;
		case kHighLevelEvent:
			AEProcessAppleEvent(&ev);
			break;
		case activateEvt:
		{
			MwLL top = from_window((WindowPtr)ev.message);

			if(top != NULL && !top->quickdraw.tool && !(ev.modifiers & activeFlag) && is_alive(focus) && get_top(focus) == top) {
				set_focus(NULL);
			}
			break;
		}
		}
	} else {
		poll_input(&ev);
	}
}

/* big buffers go into temporary memory where possible, so the partition of the application does not have to fit
 * them. the header in front of the buffer remembers where it came from */
#define BIG_HEADER 16

void* MwLLQuickdrawAllocBig(size_t size) {
	OSErr err;
	Handle h = TempNewHandle(size + BIG_HEADER, &err);
	char*  p;

	if(h != NULL && err == noErr) {
		HLock(h);
		p = *h;
	} else {
		h = NULL;
		if((p = malloc(size + BIG_HEADER)) == NULL) return NULL;
	}

	*(Handle*)p = h;
	return p + BIG_HEADER;
}

void MwLLQuickdrawFreeBig(void* ptr) {
	char*  p;
	Handle h;

	if(ptr == NULL) return;

	p = (char*)ptr - BIG_HEADER;
	h = *(Handle*)p;
	if(h != NULL) {
		DisposeHandle(h);
	} else {
		free(p);
	}
}

/* only worth it for big pixmaps, small ones get made and freed all the time (text, gradients) */
#define PIXMAP_BIG(w, h) ((long)(w) * (h) * 4 >= 64 * 1024)

static MwLLPixmap MwLLCreatePixmapImpl(MwLL handle, unsigned char* data, int width, int height) {
	MwLLPixmap r = malloc(sizeof(*r));
	PixMap*	   pm;
	int	   mask_bytes;

	(void)handle;

	if(!r) {
		out_of_memory();
		return NULL;
	}

	if(width < 1) width = 1;
	if(height < 1) height = 1;

	mask_bytes = ((width + 15) / 16) * 2;

	r->common.width		   = width;
	r->common.height	   = height;
	if(PIXMAP_BIG(width, height)) {
		r->common.raw	    = MwLLQuickdrawAllocBig(4 * width * height);
		r->quickdraw.pixels = MwLLQuickdrawAllocBig(4 * width * height);
	} else {
		r->common.raw	    = malloc(4 * width * height);
		r->quickdraw.pixels = malloc(4 * width * height);
	}
	r->quickdraw.mask.baseAddr = malloc(mask_bytes * height);

	if(!r->common.raw || !r->quickdraw.pixels || !r->quickdraw.mask.baseAddr) {
		if(PIXMAP_BIG(width, height)) {
			MwLLQuickdrawFreeBig(r->common.raw);
			MwLLQuickdrawFreeBig(r->quickdraw.pixels);
		} else {
			free(r->common.raw);
			free(r->quickdraw.pixels);
		}
		free(r->quickdraw.mask.baseAddr);
		free(r);

		out_of_memory();
		return NULL;
	}

	memcpy(r->common.raw, data, 4 * width * height);

	r->quickdraw.mask.rowBytes = mask_bytes;
	SetRect(&r->quickdraw.mask.bounds, 0, 0, width, height);

	/* hand-made 32-bit direct pixmap, much cheaper than a GWorld per pixmap */
	pm = &r->quickdraw.pixmap;
	memset(pm, 0, sizeof(*pm));
	pm->baseAddr  = (Ptr)r->quickdraw.pixels;
	pm->rowBytes  = (4 * width) | 0x8000;
	pm->hRes      = 72L << 16;
	pm->vRes      = 72L << 16;
	pm->pixelType = RGBDirect;
	pm->pixelSize = 32;
	pm->cmpCount  = 3;
	pm->cmpSize   = 8;
	pm->pmTable   = (**dummy_pixmap).pmTable;
	SetRect(&pm->bounds, 0, 0, width, height);

	MwLLPixmapUpdate(r);

	return r;
}

static void MwLLPixmapUpdateImpl(MwLLPixmap r) {
	int x, y;
	int w = r->common.width;

	r->quickdraw.opaque = 1;
	memset(r->quickdraw.mask.baseAddr, 0, r->quickdraw.mask.rowBytes * r->common.height);

	for(y = 0; y < r->common.height; y++) {
		unsigned char* m = (unsigned char*)r->quickdraw.mask.baseAddr + y * r->quickdraw.mask.rowBytes;
		for(x = 0; x < w; x++) {
			unsigned char* px = &r->common.raw[(y * w + x) * 4];
			unsigned char* q  = &r->quickdraw.pixels[(y * w + x) * 4];

			q[0] = 0;
			q[1] = px[0];
			q[2] = px[1];
			q[3] = px[2];

			if(px[3]) {
				m[x / 8] |= 0x80 >> (x % 8);
			} else {
				r->quickdraw.opaque = 0;
			}
		}
	}
}

static void MwLLDestroyPixmapImpl(MwLLPixmap pixmap) {
	if(PIXMAP_BIG(pixmap->common.width, pixmap->common.height)) {
		MwLLQuickdrawFreeBig(pixmap->common.raw);
		MwLLQuickdrawFreeBig(pixmap->quickdraw.pixels);
	} else {
		free(pixmap->common.raw);
		free(pixmap->quickdraw.pixels);
	}
	free(pixmap->quickdraw.mask.baseAddr);

	free(pixmap);
}

static void MwLLDrawPixmapImpl(MwLL handle, MwRect* rect, MwLLPixmap pixmap) {
	GrafPtr port;
	Rect	src, dst;

	if(rect->width <= 0 || rect->height <= 0) return;

	PRIM_BEGIN(handle);

	GetPort(&port);

	SetRect(&src, 0, 0, pixmap->common.width, pixmap->common.height);
	SetRect(&dst, rect->x, rect->y, rect->x + rect->width, rect->y + rect->height);

	ForeColor(blackColor);
	BackColor(whiteColor);

	if(pixmap->quickdraw.opaque) {
		CopyBits((BitMap*)&pixmap->quickdraw.pixmap, BITS(port), &src, &dst, srcCopy, NULL);
	} else {
		CopyDeepMask((BitMap*)&pixmap->quickdraw.pixmap, &pixmap->quickdraw.mask, BITS(port), &src, &src, &dst, srcCopy, NULL);
	}

	PRIM_END(handle);
}

static void MwLLSetIconImpl(MwLL handle, MwLLPixmap pixmap) {
	static unsigned short serial = 0;
	unsigned char	      rgba[16 * 16 * 4];
	IconFamilyHandle      family;
	IconRef		      icon;
	OSType		      creator, type;
	ProcessSerialNumber   psn;

	if(pixmap == NULL || pixmap->common.width < 1 || pixmap->common.height < 1) return;

	scale_icon(pixmap, rgba);

	set_apple_icon(handle, rgba);

	if(!icon_setup() || (family = make_icon_family(rgba)) == NULL) return;

	/* the registry of Icon Services is shared by all applications, so the creator comes from the process
	 * (other milsko applications would replace this icon otherwise) and the type tells the icons of it apart */
	GetCurrentProcess(&psn);
	creator = ('M' << 24) | ('w' << 16) | (psn.lowLongOfPSN & 0xffff);
	type	= ('M' << 24) | ('w' << 16) | ++serial;
	if(itbl.RegisterIconRefFromIconFamily(creator, type, family, &icon) == noErr) {
		free_icon(handle);
		handle->quickdraw.icon	       = icon;
		handle->quickdraw.icon_creator = creator;
		handle->quickdraw.icon_type    = type;

		apply_icon(handle);
	}

	DisposeHandle((Handle)family);
}

static void MwLLForceRenderImpl(MwLL handle) {
	push_event(QEVENT_DRAW, handle);
}

static void MwLLSetCursorImpl(MwLL handle, MwCursor* image, MwCursor* mask) {
	Cursor* c = handle->quickdraw.setting_drag_cursor ? &handle->quickdraw.cursor_drag : &handle->quickdraw.cursor;
	int	x, y, xs, ys, hx, hy;

	memset(c, 0, sizeof(*c));

	xs = -mask->x + image->x;
	ys = mask->height + mask->y;
	ys = image->height + image->y - ys;
	if(ys < 0) ys = -ys;

	for(y = 0; y < mask->height && y < 16; y++) {
		unsigned int l = mask->data[y];
		for(x = mask->width - 1; x >= 0; x--) {
			if((l & 1) && x < 16) c->mask[y] |= 0x8000 >> x;
			l = l >> 1;
		}
	}

	/* image pixels are white, the remaining pixels inside of the mask are black */
	for(y = 0; y < 16; y++) c->data[y] = c->mask[y];
	for(y = 0; y < image->height; y++) {
		unsigned int l = image->data[y];
		for(x = image->width - 1; x >= 0; x--) {
			int px = xs + x, py = ys + y;

			if((l & 1) && px >= 0 && px < 16 && py >= 0 && py < 16) c->data[py] &= ~(0x8000 >> px);
			l = l >> 1;
		}
	}

	hx	     = xs - image->x;
	hy	     = ys + (MwCursorDataHeight + image->y);
	c->hotSpot.h = hx < 0 ? 0 : (hx > 15 ? 15 : hx);
	c->hotSpot.v = hy < 0 ? 0 : (hy > 15 ? 15 : hy);

	handle->quickdraw.has_cursor = 1;

	/* hover only changes when the mouse moves, which it may not do for a while (e.g. when grabbed) */
	update_cursor(last_mouse);
}

static void MwLLDetachImpl(MwLL handle, MwPoint* point) {
	MwLL  parent = handle->quickdraw.parent;
	MwLL  top;
	Point o;
	int   x, y, i;

	if(parent == NULL || (top = get_top(parent)) == NULL) return;

	if(cur_draw != NULL && get_top(cur_draw) == top) leave_draw();

	window_origin(top->quickdraw.window, &o);
	get_offset(parent, &x, &y);

	expose(handle);

	for(i = 0; i < arrlen(parent->quickdraw.children); i++) {
		if(parent->quickdraw.children[i] == handle) {
			arrdel(parent->quickdraw.children, i);
			break;
		}
	}

	handle->quickdraw.parent = NULL;

	create_window(handle, o.h + x + point->x, o.v + y + point->y);
	create_gworld(handle);
	if(handle->quickdraw.tool) raise_tools();

	push_draw_tree(handle);
}

static void MwLLShowImpl(MwLL handle, int show) {
	show = show ? 1 : 0;

	if(handle->quickdraw.window != NULL) {
		handle->quickdraw.shown = show;
		if(show) {
			ShowWindow(handle->quickdraw.window);
			if(!handle->quickdraw.tool) SelectWindow(handle->quickdraw.window);
			raise_tools();
		} else {
			HideWindow(handle->quickdraw.window);
		}
		return;
	}

	if(handle->quickdraw.shown == show) return;

	if(show) {
		handle->quickdraw.shown = 1;
		push_draw_tree(handle);
	} else {
		expose(handle);
		handle->quickdraw.shown = 0;
	}
}

static void MwLLMakePopupImpl(MwLL handle, MwLL parent) {
	(void)parent;

	if(handle->quickdraw.window != NULL && handle->quickdraw.shown) SelectWindow(handle->quickdraw.window);
}

static void MwLLSetSizeHintsImpl(MwLL handle, int minx, int miny, int maxx, int maxy) {
	handle->quickdraw.min_w = minx;
	handle->quickdraw.min_h = miny;
	handle->quickdraw.max_w = maxx;
	handle->quickdraw.max_h = maxy;

	restyle(handle);
}

static void MwLLMakeBorderlessImpl(MwLL handle, int toggle) {
	handle->quickdraw.borderless = toggle;

	restyle(handle);
}

static void MwLLFocusImpl(MwLL handle) {
	MwLL top = get_top(handle);

	if(top != NULL && !top->quickdraw.tool && !((WindowPeek)top->quickdraw.window)->hilited) {
		SelectWindow(top->quickdraw.window);
		raise_tools();
	}

	set_focus(handle);
}

static void MwLLGrabPointerImpl(MwLL handle, int toggle) {
	if(toggle && get_top(handle) != NULL) {
		Point c;

		grab_center(handle, &c);
		handle->quickdraw.grabbed = 1;
		grab			  = handle;
		grab_alt		  = last_mouse;
		grab_ref		  = c;

		set_focus(handle);
		warp_mouse(c);

		/* the pointer is over handle now, so its cursor applies */
		update_cursor(c);
	} else {
		handle->quickdraw.grabbed = 0;
		if(grab == handle) grab = NULL;
	}
}

static void MwLLSetClipboardImpl(MwLL handle, const char* text, int clipboard_type) {
	char* s = MwUTF8TextToACPText(text);
	int   i;

	(void)handle;
	(void)clipboard_type;

	for(i = 0; s[i] != 0; i++) {
		if(s[i] == '\n') s[i] = '\r';
	}

	ZeroScrap();
	PutScrap(strlen(s), 'TEXT', s);

	free(s);
}

static void MwLLGetClipboardImpl(MwLL handle, int clipboard_type) {
	(void)clipboard_type;

	push_event(QEVENT_CLIPBOARD, handle);
}

static void MwLLMakeToolWindowImpl(MwLL handle) {
	handle->quickdraw.tool = 1;

	if(handle->quickdraw.window != NULL) {
		restyle(handle);
		raise_tools();
	}
}

static void MwLLGetCursorCoordImpl(MwLL handle, MwPoint* point) {
	EventRecord ev;

	(void)handle;

	EventAvail(0, &ev);

	point->x = ev.where.h;
	point->y = ev.where.v;
}

static void MwLLGetScreenSizeImpl(MwLL handle, MwRect* rect) {
	(void)handle;

	rect->x	     = qd.screenBits.bounds.left;
	rect->y	     = qd.screenBits.bounds.top;
	rect->width  = qd.screenBits.bounds.right - qd.screenBits.bounds.left;
	rect->height = qd.screenBits.bounds.bottom - qd.screenBits.bounds.top;
}

static void MwLLSetupDragAndDropImpl(MwLL handle) {
	static int done = 0;
	long	   response;

	if(done) return;
	done = 1;

	if(Gestalt(gestaltDragMgrAttr, &response) != noErr || !(response & (1 << gestaltDragMgrPresent))) return;
	if((dtbl.lib = MwDynamicOpen("DragLib")) == NULL) return;

#define DRAG_FUNC(x) \
	if((dtbl.x = MwDynamicSymbol(dtbl.lib, #x)) == NULL) { \
		MwDynamicClose(dtbl.lib); \
		dtbl.lib = NULL; \
		return; \
	}

	DRAG_FUNC(InstallTrackingHandler);
	DRAG_FUNC(InstallReceiveHandler);
	DRAG_FUNC(CountDragItems);
	DRAG_FUNC(GetDragItemReferenceNumber);
	DRAG_FUNC(GetFlavorFlags);
	DRAG_FUNC(GetFlavorData);
	DRAG_FUNC(GetDragMouse);
	DRAG_FUNC(ShowDragHilite);
	DRAG_FUNC(HideDragHilite);

	drag_rgn = NewRgn();

	/* for every window of the application */
	dtbl.InstallTrackingHandler(NewDragTrackingHandlerUPP(drag_tracking), NULL, NULL);
	dtbl.InstallReceiveHandler(NewDragReceiveHandlerUPP(drag_receive), NULL, NULL);

	handle->quickdraw.accepts_dnd = 1;

	handle->quickdraw.setting_drag_cursor = 1;
	MwLLSetCursor(handle, &MwCursorArrow, &MwCursorArrowMask);
	handle->quickdraw.setting_drag_cursor = 0;
}

static void MwLLBeginStateChangeImpl(MwLL handle) {
	(void)handle;
}

static void MwLLEndStateChangeImpl(MwLL handle) {
	(void)handle;
}

static void MwLLSetDarkThemeImpl(MwLL handle, int toggle) {
	(void)handle;
	(void)toggle;
}

static MwBool MwLLDoModernImpl(MwLL handle) {
	(void)handle;

	return MwFALSE;
}

static void MwLLRaiseImpl(MwLL handle) {
	MwLL parent = handle->quickdraw.parent;
	int  i;

	if(handle->quickdraw.window != NULL) {
		if(handle->quickdraw.tool) {
			BringToFront(handle->quickdraw.window);
		} else {
			SelectWindow(handle->quickdraw.window);
			raise_tools();
		}
		return;
	}

	if(parent == NULL) return;

	for(i = 0; i < arrlen(parent->quickdraw.children); i++) {
		if(parent->quickdraw.children[i] == handle) {
			if(i == arrlen(parent->quickdraw.children) - 1) return;

			arrdel(parent->quickdraw.children, i);
			arrput(parent->quickdraw.children, handle);
			break;
		}
	}

	push_draw_tree(handle);
}

static void MwLLClipImpl(MwLL handle, MwRect* rect) {
	if(rect == NULL) {
		handle->quickdraw.has_clip = 0;
	} else {
		handle->quickdraw.has_clip = 1;
		handle->quickdraw.clip	   = *rect;
	}

	if(cur_draw == handle) apply_clip(handle);
}

/* for the OpenGL widget: the window handle is in and where in it, NULL if handle is not visible */
WindowPtr MwLLQuickdrawGetWindow(MwLL handle, MwRect* rect, long* serial) {
	MwLL n = handle;
	MwLL top;
	int  x, y;

	if((top = get_top(handle)) == NULL || !top->quickdraw.shown) return NULL;
	for(; n->quickdraw.parent != NULL; n = n->quickdraw.parent) {
		if(!n->quickdraw.shown) return NULL;
	}

	get_offset(handle, &x, &y);
	rect->x	     = x;
	rect->y	     = y;
	rect->width  = handle->quickdraw.width;
	rect->height = handle->quickdraw.height;

	*serial = top->quickdraw.window_serial;

	return top->quickdraw.window;
}

/* for native text rendering (ATSUI): make the current port the one handle draws into */
int MwLLQuickdrawBeginText(MwLL handle) {
	return enter_draw(handle);
}

void MwLLQuickdrawEndText(MwLL handle) {
	if(!handle->quickdraw.drawing) {
		leave_draw();
		flush_draw(handle);
	}
}

/* for the OpenGL widget: the part of the window handle is visible in */
void MwLLQuickdrawGetVisibleRegion(MwLL handle, RgnHandle rgn) {
	calc_vis(handle, rgn);
}

static int MwLLQuickdrawCallInitImpl(void) {
	MenuHandle  m;
	EventRecord ev;
	static int  done = 0;

	init_toolbox();

	/* MwLibraryInit can be called more than once */
	if(done) return 0;
	done = 1;

	/* keyUp is masked out by default */
	SetEventMask(everyEvent);

	/* only sent if the SIZE resource of the application says isHighLevelEventAware */
	AEInstallEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(ae_open_application), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(ae_documents), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(ae_documents), 0, false);
	AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(ae_quit_application), 0, false);

	m = NewMenu(MENU_FILE, "\pFile");
	AppendMenu(m, "\pQuit/Q");
	InsertMenu(m, 0);

	set_apple_title("\p\024");

	vis_rgn	     = NewRgn();
	tmp_rgn	     = NewRgn();
	dummy_pixmap = NewPixMap();

	EventAvail(0, &ev);
	last_mouse     = ev.where;
	last_modifiers = ev.modifiers;

	return 0;
}

#include "call.c"
CALL(Quickdraw);

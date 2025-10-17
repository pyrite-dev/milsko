/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

typedef struct filechooser {
	MwWidget nav;
	MwWidget files;
	MwWidget addr;
	MwWidget addr_back;
	MwWidget addr_fwd;
	MwWidget addr_up;
	MwWidget filename_label;
	MwWidget filename;
	MwWidget okay;
	MwWidget cancel;

	MwLLPixmap dir;
	MwLLPixmap file;
	MwLLPixmap back;
	MwLLPixmap forward;
	MwLLPixmap up;
	MwLLPixmap computer;
} filechooser_t;

static void destroy(MwWidget handle) {
	filechooser_t* fc = handle->opaque;

	MwLLDestroyPixmap(fc->dir);
	MwLLDestroyPixmap(fc->file);
	MwLLDestroyPixmap(fc->back);
	MwLLDestroyPixmap(fc->forward);
	MwLLDestroyPixmap(fc->up);
	MwLLDestroyPixmap(fc->computer);
	free(handle->opaque);
	MwDestroyWidget(handle);
}

static void cancel(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	destroy(handle->parent);
}

static void cancel_window(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	destroy(handle);
}

static void okay(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	destroy(handle->parent);
}

static void layout(MwWidget handle) {
	filechooser_t* fc = handle->opaque;
	int	       w  = MwGetInteger(handle, MwNwidth);
	int	       h  = MwGetInteger(handle, MwNheight);
	int	       wx, wy, ww, wh;

	wx = 5;
	wy = 5 + 24 + 5;
	ww = 160;
	wh = h - 10 - 24 - 5 - 24 - 5 - 24 - 5;
	if(fc->nav == NULL) {
		fc->nav = MwVaCreateWidget(MwListBoxClass, "nav", handle, wx, wy, ww, wh,
					   MwNleftPadding, 16,
					   NULL);
		MwListBoxInsert(fc->nav, -1, fc->computer, "Home", NULL);
	} else {
		MwVaApply(fc->nav,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5 + 160 + 5;
	wy = 5 + 24 + 5;
	ww = w - 5 - 160 - 5 - 5;
	wh = h - 10 - 24 - 5 - 24 - 5 - 24 - 5;
	if(fc->files == NULL) {
		fc->files = MwVaCreateWidget(MwListBoxClass, "files", handle, wx, wy, ww, wh,
					     MwNhasHeading, 1,
					     MwNleftPadding, 16,
					     NULL);
	} else {
		MwVaApply(fc->files,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5 + ((24 + 5) * 3);
	wy = 5;
	ww = w - 10 - ((24 + 5) * 3);
	wh = 24;
	if(fc->addr == NULL) {
		fc->addr = MwCreateWidget(MwEntryClass, "addr", handle, wx, wy, ww, wh);
	} else {
		MwVaApply(fc->addr,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5;
	wy = 5;
	ww = 24;
	wh = 24;
	if(fc->addr_back == NULL) {
		fc->addr_back = MwVaCreateWidget(MwButtonClass, "addr_back", handle, wx, wy, ww, wh,
						 MwNpixmap, fc->back,
						 NULL);
	} else {
		MwVaApply(fc->addr_back,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5 + 24 + 5;
	wy = 5;
	ww = 24;
	wh = 24;
	if(fc->addr_fwd == NULL) {
		fc->addr_fwd = MwVaCreateWidget(MwButtonClass, "addr_fwd", handle, wx, wy, ww, wh,
						MwNpixmap, fc->forward,
						NULL);
	} else {
		MwVaApply(fc->addr_fwd,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5 + 24 + 5 + 24 + 5;
	wy = 5;
	ww = 24;
	wh = 24;
	if(fc->addr_up == NULL) {
		fc->addr_up = MwVaCreateWidget(MwButtonClass, "addr_up", handle, wx, wy, ww, wh,
					       MwNpixmap, fc->up,
					       NULL);
	} else {
		MwVaApply(fc->addr_up,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5 + ((24 + 5) * 3);
	wy = h - 5 - 24 - 5 - 24;
	ww = w - 10 - ((24 + 5) * 3);
	wh = 24;
	if(fc->filename == NULL) {
		fc->filename = MwCreateWidget(MwEntryClass, "filename", handle, wx, wy, ww, wh);
	} else {
		MwVaApply(fc->filename,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = 5;
	wy = h - 5 - 24 - 5 - 24;
	ww = ((24 + 5) * 3);
	wh = 24;
	if(fc->filename_label == NULL) {
		fc->filename_label = MwVaCreateWidget(MwLabelClass, "filename_label", handle, wx, wy, ww, wh,
						      MwNtext, "Filename:",
						      NULL);
	} else {
		MwVaApply(fc->filename_label,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = w - 5 - 24 * 3 - 5 - 24 * 3;
	wy = h - 5 - 24;
	ww = 24 * 3;
	wh = 24;
	if(fc->okay == NULL) {
		fc->okay = MwVaCreateWidget(MwButtonClass, "ok", handle, wx, wy, ww, wh,
					    MwNtext, "Okay",
					    NULL);

		MwAddUserHandler(fc->okay, MwNactivateHandler, okay, NULL);
	} else {
		MwVaApply(fc->okay,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}

	wx = w - 5 - 24 * 3;
	wy = h - 5 - 24;
	ww = 24 * 3;
	wh = 24;
	if(fc->cancel == NULL) {
		fc->cancel = MwVaCreateWidget(MwButtonClass, "cancel", handle, wx, wy, ww, wh,
					      MwNtext, "Cancel",
					      NULL);

		MwAddUserHandler(fc->cancel, MwNactivateHandler, cancel, NULL);
	} else {
		MwVaApply(fc->cancel,
			  MwNx, wx,
			  MwNy, wy,
			  MwNwidth, ww,
			  MwNheight, wh,
			  NULL);
	}
}

static void resize(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	layout(handle);
}

static int qsort_files(const void* a, const void* b) {
	MwDirectoryEntry* aent = *(MwDirectoryEntry**)a;
	MwDirectoryEntry* bent = *(MwDirectoryEntry**)b;

	return strcmp(aent->name, bent->name);
}

static void scan(MwWidget handle, const char* path) {
	filechooser_t*	   fc	   = handle->opaque;
	void*		   dir	   = MwDirectoryOpen(path);
	MwDirectoryEntry** entries = NULL;
	int		   i;
	char**		   names = NULL;
	char**		   dates = NULL;
	char**		   sizes = NULL;
	MwLLPixmap*	   icons = NULL;
	if(dir != NULL) {
		MwDirectoryEntry* entry;
		while((entry = MwDirectoryRead(dir)) != NULL) arrput(entries, entry);
		MwDirectoryClose(dir);

		qsort(entries, arrlen(entries), sizeof(MwDirectoryEntry*), qsort_files);
	}

	MwVaApply(fc->addr,
		  MwNtext, path,
		  NULL);

	MwListBoxReset(fc->files);
	MwListBoxInsert(fc->files, -1, NULL, "Name", "Date modified", "Size", NULL);
	MwListBoxSetWidth(fc->files, 0, -128 - 96);
	MwListBoxSetWidth(fc->files, 1, 128);
	MwListBoxSetWidth(fc->files, 2, 0);

	icons = NULL;
	names = NULL;
	dates = NULL;
	sizes = NULL;
	for(i = 0; i < arrlen(entries); i++) {
		if(strcmp(entries[i]->name, ".") == 0 || strcmp(entries[i]->name, "..") == 0) continue;
		if(entries[i]->type == MwDIRECTORY_DIRECTORY) {
			char* date = malloc(128);

			MwStringTime(date, entries[i]->mtime);

			arrput(names, entries[i]->name);
			arrput(dates, date);
			arrput(sizes, NULL);
			arrput(icons, fc->dir);
		}
	}
	for(i = 0; i < arrlen(entries); i++) {
		if(entries[i]->type == MwDIRECTORY_FILE) {
			char* date = malloc(128);
			char* size = malloc(128);

			MwStringTime(date, entries[i]->mtime);
			MwStringSize(size, entries[i]->size);

			arrput(names, entries[i]->name);
			arrput(dates, date);
			arrput(sizes, size);
			arrput(icons, fc->file);
		}
	}
	MwListBoxInsertMultiple(fc->files, -1, arrlen(names), icons, names, dates, sizes, NULL);
	for(i = 0; i < arrlen(dates); i++) {
		if(dates[i] != NULL) free(dates[i]);
	}
	for(i = 0; i < arrlen(sizes); i++) {
		if(sizes[i] != NULL) free(sizes[i]);
	}
	arrfree(sizes);
	arrfree(dates);
	arrfree(names);
	arrfree(icons);

	for(i = 0; i < arrlen(entries); i++) MwDirectoryFreeEntry(entries[i]);
	arrfree(entries);
}

MwWidget MwFileChooser(MwWidget handle, const char* title) {
	MwWidget       window;
	MwPoint	       p;
	int	       ww = MwGetInteger(handle, MwNwidth);
	int	       wh = MwGetInteger(handle, MwNheight);
	int	       w, h;
	filechooser_t* fc = malloc(sizeof(*fc));
	char*	       path;
	MwLLPixmap     icon;

	memset(fc, 0, sizeof(*fc));

	p.x = 0;
	p.y = 0;

	w      = 700;
	h      = w * 2 / 3;
	window = MwVaCreateWidget(MwWindowClass, "filechooser", handle, ww, wh, w, h,
				  MwNtitle, title,
				  NULL);

	fc->dir	     = MwLoadXPM(window, MwIconDirectory);
	fc->file     = MwLoadXPM(window, MwIconFile);
	fc->back     = MwLoadXPM(window, MwIconBack);
	fc->forward  = MwLoadXPM(window, MwIconForward);
	fc->up	     = MwLoadXPM(window, MwIconUp);
	fc->computer = MwLoadXPM(window, MwIconComputer);

	icon = MwLoadXPM(window, MwIconSearch);
	MwVaApply(window,
		  MwNiconPixmap, icon,
		  NULL);

	window->opaque = fc;

	layout(window);
	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(window, MwNcloseHandler, cancel_window, NULL);

	path = MwDirectoryCurrent();
	scan(window, path);
	free(path);

	MwLLDetach(window->lowlevel, &p);
	MwLLMakePopup(window->lowlevel, handle->lowlevel);

	return window;
}

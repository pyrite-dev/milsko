/* $Id$ */
#include <Mw/Milsko.h>

#include "../external/stb_ds.h"

typedef struct filechooser {
	char*  path;
	char** history;
	int    history_seek;
	int    dir_only;

	MwDirectoryEntry** entries;
	MwDirectoryEntry** sorted_entries;

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

static void scan(MwWidget handle, const char* path, int record);

static void destroy(MwWidget handle) {
	filechooser_t* fc = handle->opaque;
	int	       i;

	for(i = 0; i < arrlen(fc->history); i++) free(fc->history[i]);
	arrfree(fc->history);

	arrfree(fc->sorted_entries);
	for(i = 0; i < arrlen(fc->entries); i++) MwDirectoryFreeEntry(fc->entries[i]);
	arrfree(fc->entries);

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

static void msgbox_okay(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	MwMessageBoxDestroy(handle->parent);
}

static void files_activate(MwWidget handle, void* user, void* call) {
	int	       index = *(int*)call;
	filechooser_t* fc    = handle->parent->opaque;

	(void)user;

	if(fc->sorted_entries[index - 1]->type == MwDIRECTORY_DIRECTORY) {
		char* p = MwDirectoryJoin(fc->path, fc->sorted_entries[index - 1]->name);

		scan(handle->parent, p, 1);

		free(p);
	} else {
		char* p;

		p = MwDirectoryJoin(fc->path, fc->sorted_entries[MwGetInteger(fc->files, MwNvalue) - 1]->name);
		MwDispatchUserHandler(handle->parent, fc->dir_only ? MwNdirectoryChosenHandler : MwNfileChosenHandler, p);
		free(p);

		okay(fc->okay, NULL, NULL);
	}
}

static void okay_activate(MwWidget handle, void* user, void* call) {
	filechooser_t* fc = handle->parent->opaque;
	char*	       t  = (char*)MwGetText(fc->filename, MwNtext);
	struct stat    s;
	char*	       p = NULL;

	(void)user;
	(void)call;

	if(t != NULL && t[0] == '/') {
		p = MwStringDuplicate(t);
	} else {
		p = MwDirectoryJoin(fc->path, t);
	}

	if(t == NULL || strlen(t) == 0) {
		MwWidget msgbox = MwMessageBox(handle->parent, fc->dir_only ? "You have to type directory!" : "You have to type filename!", "Error", MwMB_ICONERROR | MwMB_BUTTONOK);
		MwAddUserHandler(MwMessageBoxGetChild(msgbox, MwMB_BUTTONOK), MwNactivateHandler, msgbox_okay, NULL);
	} else if(stat(p, &s) != 0) {
		MwWidget msgbox = MwMessageBox(handle->parent, "File does not exist!", "Error", MwMB_ICONERROR | MwMB_BUTTONOK);
		MwAddUserHandler(MwMessageBoxGetChild(msgbox, MwMB_BUTTONOK), MwNactivateHandler, msgbox_okay, NULL);
	} else if(fc->dir_only ? !S_ISDIR(s.st_mode) : 0) {
		MwWidget msgbox = MwMessageBox(handle->parent, "File is not permitted here!", "Error", MwMB_ICONERROR | MwMB_BUTTONOK);
		MwAddUserHandler(MwMessageBoxGetChild(msgbox, MwMB_BUTTONOK), MwNactivateHandler, msgbox_okay, NULL);
	} else {
		MwDispatchUserHandler(handle->parent, fc->dir_only ? MwNdirectoryChosenHandler : MwNfileChosenHandler, p);
		okay(fc->okay, NULL, NULL);
	}

	if(p != NULL) free(p);
}

static void nav_activate(MwWidget handle, void* user, void* call) {
	int	    index = *(int*)call;
	const char* e	  = MwListBoxGet(handle, index);

	(void)user;

	if(strcmp(e, "Home") == 0) {
#ifdef _WIN32
#else
		struct passwd* p = getpwuid(getuid());

		scan(handle->parent, p->pw_dir, 1);
#endif
	}
}

static void addr_up_activate(MwWidget handle, void* user, void* call) {
	filechooser_t* fc = handle->parent->opaque;
	char*	       p  = MwDirectoryJoin(fc->path, "..");

	(void)user;
	(void)call;

	scan(handle->parent, p, 1);

	free(p);
}

static void addr_back_activate(MwWidget handle, void* user, void* call) {
	filechooser_t* fc = handle->parent->opaque;

	(void)user;
	(void)call;

	if(fc->history_seek > 1) {
		fc->history_seek -= 2;
		scan(handle->parent, fc->history[fc->history_seek], 0);
		fc->history_seek++;
	}
}

static void addr_fwd_activate(MwWidget handle, void* user, void* call) {
	filechooser_t* fc = handle->parent->opaque;

	(void)user;
	(void)call;

	if(fc->history_seek < arrlen(fc->history)) {
		scan(handle->parent, fc->history[fc->history_seek++], 0);
	}
}

static void addr_activate(MwWidget handle, void* user, void* call) {
	(void)user;
	(void)call;

	scan(handle->parent, MwGetText(handle, MwNtext), 1);
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
		MwListBoxPacket* p = MwListBoxCreatePacket();
		int		 index;

		index = MwListBoxPacketInsert(p, -1);
		MwListBoxPacketSet(p, index, 0, "Home");
		MwListBoxPacketSetIcon(p, index, fc->computer);

		fc->nav = MwVaCreateWidget(MwListBoxClass, "nav", handle, wx, wy, ww, wh,
					   MwNleftPadding, 16,
					   NULL);
		MwListBoxInsert(fc->nav, -1, p);
		MwAddUserHandler(fc->nav, MwNactivateHandler, nav_activate, NULL);

		MwListBoxDestroyPacket(p);
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
		MwAddUserHandler(fc->files, MwNactivateHandler, files_activate, NULL);
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
		MwAddUserHandler(fc->addr, MwNactivateHandler, addr_activate, NULL);
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
		MwAddUserHandler(fc->addr_back, MwNactivateHandler, addr_back_activate, NULL);
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
		MwAddUserHandler(fc->addr_fwd, MwNactivateHandler, addr_fwd_activate, NULL);
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
		MwAddUserHandler(fc->addr_up, MwNactivateHandler, addr_up_activate, NULL);
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
		MwAddUserHandler(fc->filename, MwNactivateHandler, okay_activate, NULL);
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
						      MwNtext, fc->dir_only ? "Directory:" : "Filename:",
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

		MwAddUserHandler(fc->okay, MwNactivateHandler, okay_activate, NULL);
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

static void scan(MwWidget handle, const char* path, int record) {
	filechooser_t*	 fc  = handle->opaque;
	void*		 dir = MwDirectoryOpen(path);
	int		 i;
	MwListBoxPacket* p = MwListBoxCreatePacket();
	int		 index;

	if(dir != NULL) {
		MwDirectoryEntry* entry;

		arrfree(fc->sorted_entries);
		for(i = 0; i < arrlen(fc->entries); i++) MwDirectoryFreeEntry(fc->entries[i]);
		arrfree(fc->entries);

		while((entry = MwDirectoryRead(dir)) != NULL) arrput(fc->entries, entry);
		MwDirectoryClose(dir);

		qsort(fc->entries, arrlen(fc->entries), sizeof(MwDirectoryEntry*), qsort_files);

		if(record) {
			char* str = MwStringDuplicate(path);

			while(arrlen(fc->history) > fc->history_seek) {
				free(fc->history[fc->history_seek]);
				arrdel(fc->history, fc->history_seek);
			}

			arrins(fc->history, fc->history_seek, str);

			fc->history_seek++;
		}
	} else {
		MwWidget msgbox = MwMessageBox(handle, "Directory does not exist!", "Error", MwMB_ICONERROR | MwMB_BUTTONOK);
		MwAddUserHandler(MwMessageBoxGetChild(msgbox, MwMB_BUTTONOK), MwNactivateHandler, msgbox_okay, NULL);
		return;
	}

	if(fc->path != NULL) free(fc->path);
	fc->path = MwStringDuplicate(path);

	MwVaApply(fc->addr,
		  MwNtext, path,
		  NULL);

	if(fc->dir_only) {
		MwVaApply(fc->filename,
			  MwNtext, path,
			  NULL);
	} else {
		MwVaApply(fc->filename,
			  MwNtext, "",
			  NULL);
	}

	MwListBoxReset(fc->files);
	MwListBoxSetWidth(fc->files, 0, -128 - 96);
	MwListBoxSetWidth(fc->files, 1, 128);
	MwListBoxSetWidth(fc->files, 2, 0);

	index = MwListBoxPacketInsert(p, -1);
	MwListBoxPacketSet(p, index, -1, "Name");
	MwListBoxPacketSet(p, index, -1, "Date modified");
	MwListBoxPacketSet(p, index, -1, "Size");

	for(i = 0; i < arrlen(fc->entries); i++) {
		if(strcmp(fc->entries[i]->name, ".") == 0 || strcmp(fc->entries[i]->name, "..") == 0) continue;
		if(fc->entries[i]->type == MwDIRECTORY_DIRECTORY) {
			char date[128];

			MwStringTime(date, fc->entries[i]->mtime);

			index = MwListBoxPacketInsert(p, -1);
			MwListBoxPacketSetIcon(p, index, fc->dir);
			MwListBoxPacketSet(p, index, -1, fc->entries[i]->name);
			MwListBoxPacketSet(p, index, -1, date);
			MwListBoxPacketSet(p, index, -1, NULL);

			arrput(fc->sorted_entries, fc->entries[i]);
		}
	}
	for(i = 0; i < arrlen(fc->entries); i++) {
		if(fc->entries[i]->type == MwDIRECTORY_FILE && !fc->dir_only) {
			char date[128];
			char size[128];

			MwStringTime(date, fc->entries[i]->mtime);
			MwStringSize(size, fc->entries[i]->size);

			index = MwListBoxPacketInsert(p, -1);
			MwListBoxPacketSetIcon(p, index, fc->file);
			MwListBoxPacketSet(p, index, -1, fc->entries[i]->name);
			MwListBoxPacketSet(p, index, -1, date);
			MwListBoxPacketSet(p, index, -1, size);

			arrput(fc->sorted_entries, fc->entries[i]);
		}
	}
	MwListBoxInsert(fc->files, -1, p);
	MwListBoxDestroyPacket(p);
}

MwWidget MwFileChooserEx(MwWidget handle, const char* title, int dir) {
	MwWidget       window;
	int	       w, h;
	filechooser_t* fc = malloc(sizeof(*fc));
	char*	       path;
	MwLLPixmap     icon;
	int	       wx;
	int	       wy;

	memset(fc, 0, sizeof(*fc));

	w = 700;
	h = w * 2 / 3;

	if(handle == NULL) {
		wx = wy = MwDEFAULT;
	} else {
		wx = MwGetInteger(handle, MwNx) + (MwGetInteger(handle, MwNwidth) - w) / 2;
		wy = MwGetInteger(handle, MwNy) + (MwGetInteger(handle, MwNheight) - h) / 2;
	}

	window = MwVaCreateWidget(MwWindowClass, "filechooser", NULL, wx, wy, w, h,
				  MwNtitle, title,
				  NULL);
	if(handle != NULL) MwReparent(window, handle);

	fc->history_seek = 0;

	fc->dir	     = MwLoadIcon(window, MwIconDirectory);
	fc->file     = MwLoadIcon(window, MwIconFile);
	fc->back     = MwLoadIcon(window, MwIconBack);
	fc->forward  = MwLoadIcon(window, MwIconForward);
	fc->up	     = MwLoadIcon(window, MwIconUp);
	fc->computer = MwLoadIcon(window, MwIconComputer);
	fc->dir_only = dir;

	icon = MwLoadIcon(window, MwIconSearch);
	MwVaApply(window,
		  MwNiconPixmap, icon,
		  NULL);

	window->opaque = fc;

	layout(window);
	MwAddUserHandler(window, MwNresizeHandler, resize, NULL);
	MwAddUserHandler(window, MwNcloseHandler, cancel_window, NULL);

	path = MwDirectoryCurrent();
	scan(window, path, 1);
	free(path);

	MwLLBeginStateChange(window->lowlevel);
	MwLLMakePopup(window->lowlevel, handle == NULL ? NULL : handle->lowlevel);
	MwLLEndStateChange(window->lowlevel);

	return window;
}

MwWidget MwFileChooser(MwWidget handle, const char* title) {
	return MwFileChooserEx(handle, title, 0);
}

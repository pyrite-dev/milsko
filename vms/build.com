$ if f$search("[.src]color.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]color.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]color.obj [.src]color.c
$ endif
$ if f$search("[.src]core.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]core.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]core.obj [.src]core.c
$ endif
$ if f$search("[.src]default.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]default.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]default.obj [.src]default.c
$ endif
$ if f$search("[.src]draw.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]draw.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]draw.obj [.src]draw.c
$ endif
$ if f$search("[.src]error.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]error.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]error.obj [.src]error.c
$ endif
$ if f$search("[.src]lowlevel.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]lowlevel.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]lowlevel.obj [.src]lowlevel.c
$ endif
$ if f$search("[.src]string.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]string.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]string.obj [.src]string.c
$ endif
$ if f$search("[.src]text.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]text.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]text.obj [.src]text.c
$ endif
$ if f$search("[.src]unicode.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src]unicode.obj"
$	cc /include_directory=("./include","./src") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src]unicode.obj [.src]unicode.c
$ endif
$ if f$search("[.src.cursor]arrow.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.cursor]arrow.obj"
$	cc /include_directory=("./include","./src/cursor") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.cursor]arrow.obj [.src.cursor]arrow.c
$ endif
$ if f$search("[.src.cursor]cross.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.cursor]cross.obj"
$	cc /include_directory=("./include","./src/cursor") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.cursor]cross.obj [.src.cursor]cross.c
$ endif
$ if f$search("[.src.cursor]default.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.cursor]default.obj"
$	cc /include_directory=("./include","./src/cursor") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.cursor]default.obj [.src.cursor]default.c
$ endif
$ if f$search("[.src.cursor]hidden.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.cursor]hidden.obj"
$	cc /include_directory=("./include","./src/cursor") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.cursor]hidden.obj [.src.cursor]hidden.c
$ endif
$ if f$search("[.src.cursor]text.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.cursor]text.obj"
$	cc /include_directory=("./include","./src/cursor") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.cursor]text.obj [.src.cursor]text.c
$ endif
$ if f$search("[.src.widget]button.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]button.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]button.obj [.src.widget]button.c
$ endif
$ if f$search("[.src.widget]checkbox.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]checkbox.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]checkbox.obj [.src.widget]checkbox.c
$ endif
$ if f$search("[.src.widget]combobox.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]combobox.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]combobox.obj [.src.widget]combobox.c
$ endif
$ if f$search("[.src.widget]entry.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]entry.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]entry.obj [.src.widget]entry.c
$ endif
$ if f$search("[.src.widget]frame.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]frame.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]frame.obj [.src.widget]frame.c
$ endif
$ if f$search("[.src.widget]image.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]image.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]image.obj [.src.widget]image.c
$ endif
$ if f$search("[.src.widget]label.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]label.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]label.obj [.src.widget]label.c
$ endif
$ if f$search("[.src.widget]listbox.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]listbox.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]listbox.obj [.src.widget]listbox.c
$ endif
$ if f$search("[.src.widget]menu.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]menu.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]menu.obj [.src.widget]menu.c
$ endif
$ if f$search("[.src.widget]numberentry.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]numberentry.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]numberentry.obj [.src.widget]numberentry.c
$ endif
$ if f$search("[.src.widget]progressbar.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]progressbar.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]progressbar.obj [.src.widget]progressbar.c
$ endif
$ if f$search("[.src.widget]radiobox.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]radiobox.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]radiobox.obj [.src.widget]radiobox.c
$ endif
$ if f$search("[.src.widget]scrollbar.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]scrollbar.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]scrollbar.obj [.src.widget]scrollbar.c
$ endif
$ if f$search("[.src.widget]separator.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]separator.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]separator.obj [.src.widget]separator.c
$ endif
$ if f$search("[.src.widget]submenu.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]submenu.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]submenu.obj [.src.widget]submenu.c
$ endif
$ if f$search("[.src.widget]treeview.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]treeview.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]treeview.obj [.src.widget]treeview.c
$ endif
$ if f$search("[.src.widget]viewport.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]viewport.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]viewport.obj [.src.widget]viewport.c
$ endif
$ if f$search("[.src.widget]window.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]window.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]window.obj [.src.widget]window.c
$ endif
$ if f$search("[.src.widget]box.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.widget]box.obj"
$	cc /include_directory=("./include","./src/widget") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.widget]box.obj [.src.widget]box.c
$ endif
$ if f$search("[.src.dialog]colorpicker.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.dialog]colorpicker.obj"
$	cc /include_directory=("./include","./src/dialog") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.dialog]colorpicker.obj [.src.dialog]colorpicker.c
$ endif
$ if f$search("[.src.dialog]directorychooser.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.dialog]directorychooser.obj"
$	cc /include_directory=("./include","./src/dialog") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.dialog]directorychooser.obj [.src.dialog]directorychooser.c
$ endif
$ if f$search("[.src.dialog]filechooser.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.dialog]filechooser.obj"
$	cc /include_directory=("./include","./src/dialog") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.dialog]filechooser.obj [.src.dialog]filechooser.c
$ endif
$ if f$search("[.src.dialog]messagebox.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.dialog]messagebox.obj"
$	cc /include_directory=("./include","./src/dialog") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.dialog]messagebox.obj [.src.dialog]messagebox.c
$ endif
$ if f$search("[.src.font]boldfont.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.font]boldfont.obj"
$	cc /include_directory=("./include","./src/font") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.font]boldfont.obj [.src.font]boldfont.c
$ endif
$ if f$search("[.src.font]boldttf.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.font]boldttf.obj"
$	cc /include_directory=("./include","./src/font") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.font]boldttf.obj [.src.font]boldttf.c
$ endif
$ if f$search("[.src.font]font.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.font]font.obj"
$	cc /include_directory=("./include","./src/font") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.font]font.obj [.src.font]font.c
$ endif
$ if f$search("[.src.font]ttf.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.font]ttf.obj"
$	cc /include_directory=("./include","./src/font") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.font]ttf.obj [.src.font]ttf.c
$ endif
$ if f$search("[.src.icon]back.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]back.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]back.obj [.src.icon]back.c
$ endif
$ if f$search("[.src.icon]clock.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]clock.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]clock.obj [.src.icon]clock.c
$ endif
$ if f$search("[.src.icon]computer.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]computer.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]computer.obj [.src.icon]computer.c
$ endif
$ if f$search("[.src.icon]directory.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]directory.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]directory.obj [.src.icon]directory.c
$ endif
$ if f$search("[.src.icon]down.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]down.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]down.obj [.src.icon]down.c
$ endif
$ if f$search("[.src.icon]error.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]error.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]error.obj [.src.icon]error.c
$ endif
$ if f$search("[.src.icon]file.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]file.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]file.obj [.src.icon]file.c
$ endif
$ if f$search("[.src.icon]forward.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]forward.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]forward.obj [.src.icon]forward.c
$ endif
$ if f$search("[.src.icon]info.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]info.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]info.obj [.src.icon]info.c
$ endif
$ if f$search("[.src.icon]left.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]left.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]left.obj [.src.icon]left.c
$ endif
$ if f$search("[.src.icon]news.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]news.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]news.obj [.src.icon]news.c
$ endif
$ if f$search("[.src.icon]note.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]note.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]note.obj [.src.icon]note.c
$ endif
$ if f$search("[.src.icon]right.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]right.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]right.obj [.src.icon]right.c
$ endif
$ if f$search("[.src.icon]search.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]search.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]search.obj [.src.icon]search.c
$ endif
$ if f$search("[.src.icon]up.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]up.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]up.obj [.src.icon]up.c
$ endif
$ if f$search("[.src.icon]warning.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.icon]warning.obj"
$	cc /include_directory=("./include","./src/icon") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.icon]warning.obj [.src.icon]warning.c
$ endif
$ if f$search("[.src.abstract]directory.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.abstract]directory.obj"
$	cc /include_directory=("./include","./src/abstract") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.abstract]directory.obj [.src.abstract]directory.c
$ endif
$ if f$search("[.src.abstract]dynamic.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.abstract]dynamic.obj"
$	cc /include_directory=("./include","./src/abstract") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.abstract]dynamic.obj [.src.abstract]dynamic.c
$ endif
$ if f$search("[.src.abstract]time.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.abstract]time.obj"
$	cc /include_directory=("./include","./src/abstract") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.abstract]time.obj [.src.abstract]time.c
$ endif
$ if f$search("[.external]stb_ds.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.external]stb_ds.obj"
$	cc /include_directory=("./include","./external") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.external]stb_ds.obj [.external]stb_ds.c
$ endif
$ if f$search("[.src.backend]x11.obj;*") .eqs. ""
$ then
$	write sys$output "CC [.src.backend]x11.obj"
$	cc /include_directory=("./include","./src/backend") /define=(_MILSKO,USE_X11,NO_IMAGE) /object=[.src.backend]x11.obj [.src.backend]x11.c
$ endif
$ if f$search("[.src]MwSHR.exe;*") .eqs. ""
$ then
$	write sys$output "LINK [.src]MwSHR.exe"
$	OPEN /WRITE LINK_OPT LINK.OPT
$	WRITE LINK_OPT "[.src]color.obj"
$	WRITE LINK_OPT "[.src]core.obj"
$	WRITE LINK_OPT "[.src]default.obj"
$	WRITE LINK_OPT "[.src]draw.obj"
$	WRITE LINK_OPT "[.src]error.obj"
$	WRITE LINK_OPT "[.src]lowlevel.obj"
$	WRITE LINK_OPT "[.src]string.obj"
$	WRITE LINK_OPT "[.src]text.obj"
$	WRITE LINK_OPT "[.src]unicode.obj"
$	WRITE LINK_OPT "[.src.cursor]arrow.obj"
$	WRITE LINK_OPT "[.src.cursor]cross.obj"
$	WRITE LINK_OPT "[.src.cursor]default.obj"
$	WRITE LINK_OPT "[.src.cursor]hidden.obj"
$	WRITE LINK_OPT "[.src.cursor]text.obj"
$	WRITE LINK_OPT "[.src.widget]button.obj"
$	WRITE LINK_OPT "[.src.widget]checkbox.obj"
$	WRITE LINK_OPT "[.src.widget]combobox.obj"
$	WRITE LINK_OPT "[.src.widget]entry.obj"
$	WRITE LINK_OPT "[.src.widget]frame.obj"
$	WRITE LINK_OPT "[.src.widget]image.obj"
$	WRITE LINK_OPT "[.src.widget]label.obj"
$	WRITE LINK_OPT "[.src.widget]listbox.obj"
$	WRITE LINK_OPT "[.src.widget]menu.obj"
$	WRITE LINK_OPT "[.src.widget]numberentry.obj"
$	WRITE LINK_OPT "[.src.widget]progressbar.obj"
$	WRITE LINK_OPT "[.src.widget]radiobox.obj"
$	WRITE LINK_OPT "[.src.widget]scrollbar.obj"
$	WRITE LINK_OPT "[.src.widget]separator.obj"
$	WRITE LINK_OPT "[.src.widget]submenu.obj"
$	WRITE LINK_OPT "[.src.widget]treeview.obj"
$	WRITE LINK_OPT "[.src.widget]viewport.obj"
$	WRITE LINK_OPT "[.src.widget]window.obj"
$	WRITE LINK_OPT "[.src.widget]box.obj"
$	WRITE LINK_OPT "[.src.dialog]colorpicker.obj"
$	WRITE LINK_OPT "[.src.dialog]directorychooser.obj"
$	WRITE LINK_OPT "[.src.dialog]filechooser.obj"
$	WRITE LINK_OPT "[.src.dialog]messagebox.obj"
$	WRITE LINK_OPT "[.src.font]boldfont.obj"
$	WRITE LINK_OPT "[.src.font]boldttf.obj"
$	WRITE LINK_OPT "[.src.font]font.obj"
$	WRITE LINK_OPT "[.src.font]ttf.obj"
$	WRITE LINK_OPT "[.src.icon]back.obj"
$	WRITE LINK_OPT "[.src.icon]clock.obj"
$	WRITE LINK_OPT "[.src.icon]computer.obj"
$	WRITE LINK_OPT "[.src.icon]directory.obj"
$	WRITE LINK_OPT "[.src.icon]down.obj"
$	WRITE LINK_OPT "[.src.icon]error.obj"
$	WRITE LINK_OPT "[.src.icon]file.obj"
$	WRITE LINK_OPT "[.src.icon]forward.obj"
$	WRITE LINK_OPT "[.src.icon]info.obj"
$	WRITE LINK_OPT "[.src.icon]left.obj"
$	WRITE LINK_OPT "[.src.icon]news.obj"
$	WRITE LINK_OPT "[.src.icon]note.obj"
$	WRITE LINK_OPT "[.src.icon]right.obj"
$	WRITE LINK_OPT "[.src.icon]search.obj"
$	WRITE LINK_OPT "[.src.icon]up.obj"
$	WRITE LINK_OPT "[.src.icon]warning.obj"
$	WRITE LINK_OPT "[.src.abstract]directory.obj"
$	WRITE LINK_OPT "[.src.abstract]dynamic.obj"
$	WRITE LINK_OPT "[.src.abstract]time.obj"
$	WRITE LINK_OPT "[.external]stb_ds.obj"
$	WRITE LINK_OPT "[.src.backend]x11.obj"
$	WRITE LINK_OPT "SYS$LIBRARY:DECW$XLIBSHR/SHARE/SHARE"
$	WRITE LINK_OPT "SYS$LIBRARY:DECC$SHR/SHARE/SHARE"
$	CLOSE LINK_OPT
$	link /SHAREABLE=[.src]MwSHR.exe LINK.OPT/options
$	DELETE LINK.OPT;*
$ endif

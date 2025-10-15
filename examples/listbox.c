/* $Id$ */
#include <Mw/Milsko.h>

#include "../resource/harvard.c"

int main(){
        MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
                MwNtitle, "test",
        NULL);
        MwWidget lb = MwCreateWidget(MwListBoxClass, "listbox", w, 5, 5, 630, 470);

        MwListBoxInsertMultiple(lb, -1, (char**)harvard, sizeof(harvard) / sizeof(harvard[0]) - 1);

        MwLoop(w);
}

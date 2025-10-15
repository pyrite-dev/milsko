/* $Id$ */
#include <Mw/Milsko.h>

int main(){
        MwWidget w = MwVaCreateWidget(MwWindowClass, "main", NULL, MwDEFAULT, MwDEFAULT, 640, 480,
                MwNtitle, "test",
        NULL);
        MwWidget lb = MwCreateWidget(MwListBoxClass, "listbox", w, 5, 5, 630, 470);
        char* str[512];
        int i;

        for(i = 0; i < sizeof(str) / sizeof(str[0]); i++){
                str[i] = malloc(16);
                sprintf(str[i], "element %d", i);
        }

        MwListBoxInsertMultiple(lb, -1, str, sizeof(str) / sizeof(str[0]));

        for(i = 0; i < sizeof(str) / sizeof(str[0]); i++){
                free(str[i]);
        }

        MwLoop(w);
}

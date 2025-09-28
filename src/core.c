/* $Id$ */
#include <Milsko/Milsko.h>

HMILSKO MilskoCreateWidget(HMILSKO parent, int x, int y, unsigned int width, unsigned int height) {
	HMILSKO h = malloc(sizeof(*h));
}

void MilskoDestroyWidget(HMILSKO handle) {
	free(handle);
}

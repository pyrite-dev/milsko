/* $Id$ */
#include <Mw/Milsko.h>

char* MwStringDupliacte(const char* str) {
	char* r = malloc(strlen(str) + 1);
	strcpy(r, str);

	return r;
}

char* MwStringConcat(const char* str1, const char* str2) {
	char* r = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(r, str1);
	strcat(r, str2);

	return r;
}

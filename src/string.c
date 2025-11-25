/* $Id$ */
#include <Mw/Milsko.h>

char* MwStringDuplicate(const char* str) {
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

void MwStringSize(char* out, MwOffset size) {
	if(size / 1024 == 0) {
		sprintf(out, "%d", (int)size);
	} else if(size / 1024 / 1024 == 0) {
		sprintf(out, "%.1fK", (double)size / 1024);
	} else if(size / 1024 / 1024 / 1024 == 0) {
		sprintf(out, "%.1fM", (double)size / 1024 / 1024);
	} else {
		sprintf(out, "%.1fG", (double)size / 1024 / 1024 / 1024);
	}
}

void MwStringTime(char* out, time_t t) {
	struct tm*  tm	     = localtime(&t);
	const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if(tm == NULL) {
		sprintf(out, "localtime error");
	} else {
		sprintf(out, "%s %2d %02d:%02d %d", months[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min, 1900 + tm->tm_year);
	}
}

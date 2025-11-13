/* $Id$ */
#include <Mw/Milsko.h>

void* MwDynamicOpen(const char* path) {
#ifdef _WIN32
	return LoadLibrary(path);
#else
	return dlopen(path, RTLD_LAZY | RTLD_LOCAL);
#endif
}

void* MwDynamicSymbol(void* handle, const char* symbol) {
#ifdef _WIN32
	return GetProcAddress(handle, symbol);
#else
	return dlsym(handle, symbol);
#endif
}

void MwDynamicClose(void* handle) {
#ifdef _WIN32
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif
}

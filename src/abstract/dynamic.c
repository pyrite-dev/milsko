#include <Mw/Milsko.h>

#if defined(_WIN32)
void* MwDynamicOpen(const char* path) {
	return LoadLibrary(path);
}

void* MwDynamicSymbol(void* handle, const char* symbol) {
	return GetProcAddress(handle, symbol);
}

void MwDynamicClose(void* handle) {
	FreeLibrary(handle);
}
#elif defined(__unix__) || defined(__APPLE__)
void* MwDynamicOpen(const char* path) {
	return dlopen(path, RTLD_LOCAL | RTLD_LAZY);
}

void* MwDynamicSymbol(void* handle, const char* symbol) {
	return dlsym(handle, symbol);
}

void MwDynamicClose(void* handle) {
	dlclose(handle);
}
#endif

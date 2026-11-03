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
#elif defined(macintosh)
static void to_pstring(unsigned char* out, const char* in, size_t max) {
	size_t l = strlen(in);

	if(l > max) l = max;
	out[0] = l;
	memcpy(out + 1, in, l);
}

void* MwDynamicOpen(const char* path) {
	CFragConnectionID conn;
	Ptr		  main_addr;
	Str255		  err;
	Str63		  name;

	to_pstring(name, path, 63);
	if(GetSharedLibrary(name, kPowerPCCFragArch, kReferenceCFrag, &conn, &main_addr, err) != noErr) return NULL;

	return (void*)conn;
}

void* MwDynamicSymbol(void* handle, const char* symbol) {
	Ptr		 addr;
	CFragSymbolClass cls;
	Str255		 name;

	to_pstring(name, symbol, 255);
	if(FindSymbol((CFragConnectionID)handle, name, &addr, &cls) != noErr) return NULL;

	return (void*)addr;
}

void MwDynamicClose(void* handle) {
	CFragConnectionID conn = (CFragConnectionID)handle;

	CloseConnection(&conn);
}
#elif defined(__APPLE__)
void* MwDynamicOpen(const char* path) {
	return (void*)NSAddImage(path, NSADDIMAGE_OPTION_RETURN_ON_ERROR | NSADDIMAGE_OPTION_WITH_SEARCHING);
}

void* MwDynamicSymbol(void* handle, const char* symbol) {
	NSSymbol sym  = NULL;
	char*	 name = MwStringConcat("_", symbol);

	if(handle == NULL) {
		if(NSIsSymbolNameDefined(name)) sym = NSLookupAndBindSymbol(name);
	} else {
		sym = NSLookupSymbolInImage(handle, name, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
	}
	free(name);

	if(sym == NULL) return NULL;
	return NSAddressOfSymbol(sym);
}

void MwDynamicClose(void* handle) {
	(void)handle;
}
#elif defined(__unix__) || defined(__HAIKU__)
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

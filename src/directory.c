/* $Id$ */
#include <Mw/Milsko.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
typedef struct dir {
	HANDLE		hFind;
	WIN32_FIND_DATA ffd;
	int		first;
} dir_t;
#else
typedef struct dir {
	DIR* dir;
} dir_t;
#endif

void* MwDirectoryOpen(const char* path) {
	dir_t* dir = malloc(sizeof(*dir));
#ifdef _WIN32
	char* p = malloc(strlen(path) + 2 + 1);
	strcpy(p, path);
	strcat(p, "/*");
	if((dir->hFind = FindFirstFile(p, &dir->ffd)) == INVALID_HANDLE_VALUE) {
		free(p);
		free(dir);
		return NULL;
	}
	free(p);
	dir->first = 1;
#else
	if((dir->dir = opendir(path)) == NULL) {
		free(dir);
		return NULL;
	}
#endif

	return dir;
}

void MwDirectoryClose(void* handle) {
	dir_t* dir = handle;
#ifdef _WIN32
	FindClose(dir->hFind);
#else
	closedir(dir->dir);
#endif
	free(handle);
}

MwDirectoryEntry* MwDirectoryRead(void* handle) {
	dir_t*		  dir	= handle;
	MwDirectoryEntry* entry = malloc(sizeof(*entry));
#ifdef _WIN32
	if(dir->first) {
		dir->first = 0;
	} else if(FindNextFile(dir->hFind, &dir->ffd) == 0) {
		free(entry);
		return NULL;
	}
	entry->name = malloc(strlen(dir->ffd.cFileName) + 1);
	strcpy(entry->name, dir->ffd.cFileName);
#else
	struct dirent* d;
	if((d = readdir(dir->dir)) == NULL) {
		free(entry);
		return NULL;
	}
	entry->name = malloc(strlen(d->d_name) + 1);
	strcpy(entry->name, d->d_name);
#endif

	return entry;
}

void MwDirectoryFreeEntry(MwDirectoryEntry* entry) {
	free(entry->name);
	free(entry);
}

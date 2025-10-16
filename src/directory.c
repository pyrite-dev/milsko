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
	DIR*  dir;
	char* base;
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
	dir->base = malloc(strlen(path) + 1);
	strcpy(dir->base, path);
#endif

	return dir;
}

void MwDirectoryClose(void* handle) {
	dir_t* dir = handle;
#ifdef _WIN32
	FindClose(dir->hFind);
#else
	closedir(dir->dir);
	free(dir->base);
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
	entry->name = MwStringDupliacte(dir->ffd.cFileName);

	if(dir->ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		entry->type = MwDIRECTORY_DIRECTORY;
	} else {
		entry->type = MwDIRECTORY_FILE;
	}
#else
	struct dirent* d;
	struct stat    s;
	char*	       p;
	if((d = readdir(dir->dir)) == NULL) {
		free(entry);
		return NULL;
	}
	entry->name = MwStringDupliacte(d->d_name);

	p = malloc(strlen(dir->base) + 1 + strlen(d->d_name) + 1);
	strcpy(p, dir->base);
	strcat(p, "/");
	strcat(p, d->d_name);

	stat(p, &s);
	if(S_ISDIR(s.st_mode)) {
		entry->type = MwDIRECTORY_DIRECTORY;
	} else {
		entry->type = MwDIRECTORY_FILE;
	}

	free(p);
#endif

	return entry;
}

void MwDirectoryFreeEntry(MwDirectoryEntry* entry) {
	free(entry->name);
	free(entry);
}

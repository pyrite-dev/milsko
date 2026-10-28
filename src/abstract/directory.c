#include <Mw/Milsko.h>

#ifdef macintosh
#include <Files.h>
#include <Finder.h>
#endif

#ifdef _WIN32
typedef struct dir {
	HANDLE		hFind;
	WIN32_FIND_DATA ffd;
	int		next;
} dir_t;
#elif defined(macintosh)
/* paths are absolute HFS paths ("Volume:Folder:File"), ":" lists the mounted volumes */
typedef struct dir {
	int   volumes;
	short vref;
	long  dirid;
	short index;
} dir_t;

static void to_pstring(unsigned char* out, const char* in) {
	char*  s = MwUTF8TextToACPText(in);
	size_t l = strlen(s);

	if(l > 255) l = 255;
	out[0] = l;
	memcpy(out + 1, s, l);

	free(s);
}

static char* from_pstring(const unsigned char* in) {
	char buf[256];

	memcpy(buf, in + 1, in[0]);
	buf[in[0]] = 0;

	return MwACPTextToUTF8Text(buf);
}

/* seconds since 1904 to seconds since 1970 */
#define MAC_EPOCH_DIFF 2082844800UL
#else
typedef struct dir {
	DIR*  dir;
	char* base;
} dir_t;
#endif

void* MwDirectoryOpen(const char* path) {
	dir_t* dir = malloc(sizeof(*dir));
#ifdef _WIN32
	char* p;
#endif
	if(!dir) {
		printf("Out Of Memory\n");
		return NULL;
	}
#ifdef _WIN32
	p = malloc(strlen(path) + 2 + 1);

	strcpy(p, path);
	if(strchr(path, '/') != NULL) {
		strcat(p, "/");
	} else {
		strcat(p, "\\");
	}
	strcat(p, "*");
	if((dir->hFind = FindFirstFile(p, &dir->ffd)) == INVALID_HANDLE_VALUE) {
		free(p);
		free(dir);
		return NULL;
	}
	free(p);
	dir->next = 1;
#elif defined(macintosh)
	dir->index   = 1;
	dir->volumes = path[0] == 0 || strcmp(path, ":") == 0;
	if(!dir->volumes) {
		FSSpec	  spec;
		CInfoPBRec pb;
		Str255	  name;

		to_pstring(name, path);
		if(FSMakeFSSpec(0, 0, name, &spec) != noErr) {
			free(dir);
			return NULL;
		}

		memset(&pb, 0, sizeof(pb));
		pb.dirInfo.ioNamePtr   = spec.name;
		pb.dirInfo.ioVRefNum   = spec.vRefNum;
		pb.dirInfo.ioDrDirID   = spec.parID;
		pb.dirInfo.ioFDirIndex = 0;
		if(PBGetCatInfoSync(&pb) != noErr || !(pb.dirInfo.ioFlAttrib & ioDirMask)) {
			free(dir);
			return NULL;
		}

		dir->vref  = spec.vRefNum;
		dir->dirid = pb.dirInfo.ioDrDirID;
	}
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
#elif defined(macintosh)
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
	ULARGE_INTEGER* l;
#elif defined(macintosh)
#else
	struct dirent* d;
	struct stat    s;
	char*	       p;
#endif

	if(!entry) {
		printf("Out Of Memory\n");
		return NULL;
	}
#ifdef _WIN32
	if(!dir->next) return NULL;

	entry->name = MwStringDuplicate(dir->ffd.cFileName);
	if(dir->ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		entry->type = MwDIRECTORY_DIRECTORY;
	} else {
		entry->type = MwDIRECTORY_FILE;
	}
	entry->size = 0;

	entry->size = entry->size << 32;
	entry->size |= dir->ffd.nFileSizeHigh;
	entry->size = entry->size << 32;
	entry->size |= dir->ffd.nFileSizeLow;

	l = (ULARGE_INTEGER*)&dir->ffd.ftLastWriteTime;

	entry->mtime = l->QuadPart / 10000000 - 11644473600;

	dir->next = FindNextFile(dir->hFind, &dir->ffd);
#elif defined(macintosh)
	if(dir->volumes) {
		HParamBlockRec pb;
		Str255	       name;

		memset(&pb, 0, sizeof(pb));
		pb.volumeParam.ioNamePtr  = name;
		pb.volumeParam.ioVolIndex = dir->index++;
		if(PBHGetVInfoSync(&pb) != noErr) {
			free(entry);
			return NULL;
		}

		entry->name  = from_pstring(name);
		entry->type  = MwDIRECTORY_DIRECTORY;
		entry->size  = 0;
		entry->mtime = pb.volumeParam.ioVLsMod - MAC_EPOCH_DIFF;
	} else {
		CInfoPBRec pb;
		Str255	   name;

		do {
			memset(&pb, 0, sizeof(pb));
			pb.hFileInfo.ioNamePtr	 = name;
			pb.hFileInfo.ioVRefNum	 = dir->vref;
			pb.hFileInfo.ioDirID	 = dir->dirid;
			pb.hFileInfo.ioFDirIndex = dir->index++;
			if(PBGetCatInfoSync(&pb) != noErr) {
				free(entry);
				return NULL;
			}

			/* skip what the Finder hides, like "Desktop DB" */
		} while((pb.hFileInfo.ioFlAttrib & ioDirMask) ? (pb.dirInfo.ioDrUsrWds.frFlags & kIsInvisible) : (pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsInvisible));

		entry->name = from_pstring(name);
		if(pb.hFileInfo.ioFlAttrib & ioDirMask) {
			entry->type  = MwDIRECTORY_DIRECTORY;
			entry->size  = 0;
			entry->mtime = pb.dirInfo.ioDrMdDat - MAC_EPOCH_DIFF;
		} else {
			entry->type  = MwDIRECTORY_FILE;
			entry->size  = pb.hFileInfo.ioFlLgLen + pb.hFileInfo.ioFlRLgLen;
			entry->mtime = pb.hFileInfo.ioFlMdDat - MAC_EPOCH_DIFF;
		}
	}
#else
	if((d = readdir(dir->dir)) == NULL) {
		free(entry);
		return NULL;
	}
	entry->name = MwStringDuplicate(d->d_name);

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
	entry->size  = s.st_size;
	entry->mtime = s.st_mtime;

	free(p);
#endif

	return entry;
}

void MwDirectoryFreeEntry(MwDirectoryEntry* entry) {
	free(entry->name);
	free(entry);
}

char* MwDirectoryCurrent(void) {
#ifdef _WIN32
	int   len = GetCurrentDirectory(0, NULL);
	char* out = malloc(len);

	GetCurrentDirectory(len, out);

	return out;
#elif defined(macintosh)
	/* walk up from the default directory, which is the folder of the application */
	char* out = MwStringDuplicate("");
	short vref;
	long  dirid;

	if(HGetVol(NULL, &vref, &dirid) != noErr) return out;

	while(1) {
		CInfoPBRec pb;
		Str255	   name;
		char*	   n;
		char*	   joined;

		memset(&pb, 0, sizeof(pb));
		pb.dirInfo.ioNamePtr   = name;
		pb.dirInfo.ioVRefNum   = vref;
		pb.dirInfo.ioDrDirID   = dirid;
		pb.dirInfo.ioFDirIndex = -1;
		if(PBGetCatInfoSync(&pb) != noErr) break;

		n      = from_pstring(name);
		joined = malloc(strlen(n) + 1 + strlen(out) + 1);
		strcpy(joined, n);
		strcat(joined, ":");
		strcat(joined, out);
		free(n);
		free(out);
		out = joined;

		if(dirid == fsRtDirID) break;
		dirid = pb.dirInfo.ioDrParID;
	}

	/* "Volume:Folder:" -> "Volume:Folder", but keep "Volume:" */
	if(strlen(out) > 1 && out[strlen(out) - 1] == ':' && strchr(out, ':') != out + strlen(out) - 1) out[strlen(out) - 1] = 0;

	return out;
#else
	return getcwd(NULL, 0);
#endif
}

#ifdef _WIN32
#define DIRSEP '\\'
#elif defined(macintosh)
#define DIRSEP ':'
#else
#define DIRSEP '/'
#endif
static void MwDirectoryJoinSingle(char* target, char* p) {
	int i;
	if(strcmp(p, ".") == 0) return;

	for(i = strlen(target) - 1; i >= 0; i--) {
		if(target[i] == DIRSEP) {
			target[i] = 0;
		} else {
			break;
		}
	}

	if(strcmp(p, "..") == 0) {
		for(i = strlen(target) - 1; i >= 0; i--) {
			if(target[i] != DIRSEP) {
				target[i] = 0;
			} else {
				break;
			}
		}
	} else {
		char b[2];
		b[0] = DIRSEP;
		b[1] = 0;

#ifdef macintosh
		/* below the volume list, a leading ':' would make the path relative */
		if(target[0] != 0)
#endif
			strcat(target, b);
		strcat(target, p);
	}

	for(i = strlen(target) - 1; i >= 0; i--) {
		if(target[i] == DIRSEP) {
			target[i] = 0;
		} else {
			break;
		}
	}

	if(strchr(target, DIRSEP) == NULL) {
		char b[2];
		b[0] = DIRSEP;
		b[1] = 0;

		strcat(target, b);
	}
}

char* MwDirectoryJoin(char* a, char* b) {
	char* p	   = malloc(strlen(a) + 3 + strlen(b));
	char* bdup = MwStringDuplicate(b);
	char* b2   = bdup;
	int   i;

	strcpy(p, a);

	for(i = strlen(p) - 1; i >= 0; i--) {
		if(p[i] == DIRSEP) {
			p[i] = 0;
		} else {
			break;
		}
	}
	while(b2 != NULL) {
		char* current = b2;

		b2 = strchr(b2, DIRSEP);
		if(b2 != NULL) {
			b2[0] = 0;
		}

		MwDirectoryJoinSingle(p, current);

		if(b2 != NULL) b2++;
	}
	free(bdup);

	return p;
}

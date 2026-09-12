#include <Mw/Milsko.h>

int MwLLWaylandDoRoundness(MwLL handle) {
	if(handle->common.type == MwLLBackendWayland) {
		return handle->wayland.on_hyprland;
	} else {
		return 0;
	};
};

int MwLLWaylandHyprlandGetRoundness(void) {
	char* home     = getenv("HOME");
	int   fallback = 20;
	char  path[PATH_MAX];
	int   roundness = fallback;
	char  line[512];
	FILE* f = NULL;

	if(home == NULL) {
		return fallback;
	}

	snprintf(path, sizeof(path), "%s/.config/hypr/hyprland.lua", home);

	f = fopen(path, "r");
	if(f == NULL) {
		return fallback;
	}

	while(fgets(line, sizeof(line), f) != NULL) {
		char* key = strstr(line, "roundness");
		char* eq;
		char* end;
		long  value;

		if(key == NULL) {
			continue;
		}

		eq = strchr(key, '=');
		if(eq == NULL) {
			continue;
		}
		eq++;

		while(*eq != '\0' && isspace((unsigned char)*eq)) {
			eq++;
		}

		value = strtol(eq, &end, 10);
		if(end == eq) {
			continue;
		}

		roundness = (int)value;
	}

	fclose(f);
	return roundness;
}

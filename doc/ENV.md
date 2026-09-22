# Environment vaiables

Platforms that support both X11 and Wayland (Linux, namely) support launching Milsko programs with following environment variables:

- `MW_BACKEND`: set to either `wayland` or `x11` to force which display protocol Wayland uses
- `MW_FORCE_CSD`: set to force the wayland backend to use CSD instead of SSD
- `MW_LIGHT_THEME`/`MW_DARK_THEME`: set to have either wayland or x11 ignore the system's appearence settings and either just use light theme/dark theme.

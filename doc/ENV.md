# Environment vaiables

Milsko programs can be optionally configured at runtime using the following environment variables:

- `MW_LIGHT_THEME`/`MW_DARK_THEME`: ignore the system's appearence settings and either use light theme or dark theme respectively .
- `MW_BACKEND` (Linux/X11 only): set to either `wayland` or `x11` to force which display protocol Wayland uses
- `MW_FORCE_CSD` (Linux/X11 only): set to force the wayland backend to use CSD instead of SSD

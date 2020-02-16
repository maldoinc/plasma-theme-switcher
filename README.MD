# plasma-theme — a KDE Plasma theme switcher

Quickly change plasma color schemes and widget styles from the command-line.

---

## Installation

Grab the binary from the [latest release](https://github.com/maldoinc/plasma-theme-switcher/releases) and move it to a directory within the path.

## Why not use lookandfeeltool?

Lookandfeeltool currently has no option to allow only partial application of a theme. What this tool will do for you is change color scheme, widget style and leave the rest of your desktop intact without changing splash screens, cursors etc.

## Where can I find widgetStyle names

To list built-in widget styles use the following command. Note that not all of them can be used.
`$ cat /usr/share/kstyle/themes/*.themerc | grep WidgetStyle | cut -d= -f2`

Additional values are `kvantum` and `kvantum-dark` and `QtCurve` for users of the respective theme engines.
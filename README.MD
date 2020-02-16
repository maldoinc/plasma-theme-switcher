# plasma-theme — a KDE Plasma theme switcher

Quickly change plasma color schemes and widget styles from the command-line.

---

## Example usage


```bash
# change color scheme to breezedark and breeze widget style.
$ plasma-theme --colors /usr/share/color-schemes/BreezeDark.colors --widgetStyle Breeze

# toggle between light/dark mode
# if the [--colors, -c] or [--widgetStyle, -w] arguments are passed twice
# the app will cycle through them
$ plasma-theme \
    -c /usr/share/color-schemes/BreezeDark.colors \
    -c /usr/share/color-schemes/BreezeLight.colors \
    -w Breeze

# for kvantum users, kvantum provides "kvantum" and "kvantum-dark" if the theme supports it.
# current example using theme KvArc
$ plasma-theme \
    -w kvantum -w kvantum-dark \
    -c ~/.local/share/color-schemes/KvArc.colors \
    -c ~/.local/share/color-schemes/KvArcDark.colors

# on some themes the toolbar is always dark, so there might be no need to change color scheme.
# In this case cycling through the two widget styles will suffice
$ plasma-theme -w kvantum -w kvantum-dark
```

You can register any of the above commands to execute at a certain time or bind them to a global shortcut.

## Installation

Grab the binary from the [latest release](https://github.com/maldoinc/plasma-theme-switcher/releases) and move it to a directory within the path.

## Q&A
### Why not use lookandfeeltool?

Lookandfeeltool currently has no option to allow only partial application of a theme. What this tool will do for you is change color scheme, widget style and leave the rest of your desktop intact without changing splash screens, cursors etc.

### Where can I find installed color schemes?

Color scheme files are located in `~/.local/share/color-schemes/` and `/usr/share/color-schemes/`.

### Where can I find widgetStyle names?

To list built-in widget styles use the following command. Note that not all of them can be used.

```bash
$ cat /usr/share/kstyle/themes/*.themerc | grep WidgetStyle | cut -d= -f2
```

Additional values are `kvantum` and `kvantum-dark` and `QtCurve` for users of the respective theme engines.
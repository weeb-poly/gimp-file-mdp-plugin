# MDP GIMP Plugin

An attempt to add MDIPack support to GIMP via plugin.

## About

### MDIPack

The MDIPack file format is used by applications such as mdiapp+, FireAlpaca, MediBang Paint, LayerPaint HD, and probably more.


### Plugin

I wanted to make an open source implementation of the file format that integrates properly with a well known open source Image Editing Application.

I made a plugin for Krita ([krita-plugin-mdp](https://github.com/weeb-poly/krita-plugin-mdp)) that worked as a POC, but adding proper impex support to Krita requires compiling from source.

GIMP has a decent Plugin API which allows for me to add proper support for the file format.

## Build

I am currently using libgimp3 and MSYS2 MINGW64. This plugin is probably going to remain incomplete until GIMP 3 has a release date.

## Ideas

### Frida-based Mode

I'm looking into the possibility of using [Frida](https://frida.re/docs/c-api/) to use FireAlpaca in the background.

The issues with this is that:
- I don't know how to use Frida (this is the main issue)
- The plugin wouldn't work Linux unless Frida can run under WINE
- The plugin would require different code for macOS (since the application works differently)
- If we read the data from memory:
  - Lots of RE required to figure out structure
- If we use the PSD export feature:
  - Potential Data loss in conversion from MDP to PSD
  - No way to call the [file-psd](https://gitlab.gnome.org/GNOME/gimp/-/tree/master/plug-ins/file-psd) plugin from another GIMP plugin

## Notes

mdiapp+ is developed by nattou.org and published by PGN Inc.
FireAlpaca is developed and published by PGN Inc.

I believe this plugin is compliant under FireAlpaca's Terms of Service as of the time this is being published.

I would like to thank Um6ra1 for his notes on the file format and Bowserinator for translating said notes from Japanese to English.

I would like to thank 42aruaour for directing me to the nattou.org group, who created the spec as well as most of applications that use it.

Finally, I would like to thank rsuzaki for sharing a lot of information on the MDIPack file format ([link](https://github.com/rsuzaki/mdp_format/wiki)). This information was extremely helpful in order to get this plugin to the state that it is.
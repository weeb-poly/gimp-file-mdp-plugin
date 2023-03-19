project('file-mdp', 'c',
  license : 'ISC',
  default_options: [
    'c_std=gnu11',
    'warning_level=1'
  ]
)

# TODO: Link Snappy and FastLZ (probably through subprojects)
zlib_dep = dependency('zlib')
libxml2_dep = dependency('libxml-2.0')


# mingw-w64-gimp3
libgimp_dep = dependency('gimp-3.0')
libgimpthumb_dep = dependency('gimpthumb-3.0')
#libgimpui_dep = dependency('gimpui-3.0')

gimpplugindir = libgimp_dep.get_pkgconfig_variable('gimplibdir')

subdir('src')

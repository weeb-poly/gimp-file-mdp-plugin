plugin_name = 'file-mdp'

plugin_sources = [
  'file-mdp.c',
  'file-mdp-load.c',
  'file-mdp-load.c',
  'file-mdp-thumb-load.c',
  'mdp-parse.c',
  'mdp-util.c'
]

# Setting win_subsystem to 'windows' builds the plugin as a GUI app
# (no console is spawn). Adding '/ENTRY:mainCRTStartup' to the link_args means
# that we don't need to define 'WinMain' (currently handled by the
# 'GIMP_MAIN' macro). This removes our dependance on the GIMP_MAIN macro and
# makes it easier to switch languages at some point in the future.

cc = meson.get_compiler('c')
link_args = []
if cc.get_define('_MSC_VER') != ''
  link_args += ['/ENTRY:mainCRTStartup']
endif


if host_machine.system() == 'windows'
  windows = import('windows')
  plugin_sources += windows.compile_resources(
    'res/gimp-plug-ins.rc',
    args: [
      '--define', 'ORIGINALFILENAME_STR="@0@"'.format(plugin_name+'.exe'),
      '--define', 'INTERNALNAME_STR="@0@"'    .format(plugin_name),
    ],
    depend_files: ['res/plug-ins.ico', 'res/gimp.manifest'],
  )
endif


add_project_arguments('-DG_LOG_DOMAIN="file-mdp"', language : 'c')


executable(plugin_name,
  plugin_sources,
  dependencies: [
#    libgimpui_dep,
    libgimp_dep,
    libgimpthumb_dep,
    zlib_dep,
    libxml2_dep
  ],
  install: true,
  install_dir: gimpplugindir / 'plug-ins' / plugin_name,
  link_args: link_args,
  win_subsystem: 'windows',
)

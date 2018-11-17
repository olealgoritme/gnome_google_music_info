google_music_info_sources = [
  'main.c',
  'google_music_info-window.c',
]

google_music_info_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

google_music_info_sources += gnome.compile_resources('google_music_info-resources',
  'google_music_info.gresource.xml',
  c_name: 'google_music_info'
)

executable('google_music_info', google_music_info_sources,
  dependencies: google_music_info_deps,
  install: true,
)

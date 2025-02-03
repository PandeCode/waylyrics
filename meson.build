project(
    'waybar_cffi_example', 'cpp',
    version: '0.1.0',
    license: 'MIT',
)

gtk            = dependency('gtk+-3.0')
libcurl        = dependency('libcurl')
epoxy          = dependency('epoxy')
glm            = dependency('glm')
sdbus          = dependency('sdbus-c++')

shared_library('waylyrics'
    ['./waylyrics/waylyrics.cpp'],
    dependencies: [ libcurl, gtk,  sdbus ],
    name_prefix: ''
)

shared_library('wb_opengl',
    ['./opengl/opengl.cpp'],
    dependencies: [ libcurl, gtk,  sdbus, glm, epoxy ],
    name_prefix: ''
)

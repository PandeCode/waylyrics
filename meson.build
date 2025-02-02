project(
    'waybar_cffi_example', 'cpp',
    version: '0.1.0',
    license: 'MIT',
)

shared_library('wb_cffi',
    ['waylyrics.cpp'],
    dependencies: [
        dependency('gtk+-3.0', version : ['>=3.22.0']),
        dependency('libcurl'),
        dependency('sdbus-c++')
    ],
    name_prefix: ''
)

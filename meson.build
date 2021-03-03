project('particles', 'cpp', default_options : 'std=c++11 -g3')

incdir = 'include/'
pfx = 'src/'

sources = [
    pfx + 'main.cpp',
    pfx + 'particle_source.cpp',
    pfx + 'shader.cpp',
]

glm = dependency('glm', version : '>=0.9.9')

executable('demo', sources,
include_directories : incdir,
dependencies : [
    glm
])
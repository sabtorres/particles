project('particles', 'cpp', default_options : 'std=c++11 -g3')

cc = meson.get_compiler('cpp')
incdir = 'include/'
pfx = 'src/'
imgui_pfx = 'src/imgui/'

sources = [
    pfx + 'main.cpp',
    pfx + 'glad.cpp',
    pfx + 'lodepng.cpp',
    imgui_pfx + 'imgui.cpp',
    imgui_pfx + 'imgui_impl_glfw.cpp',
    imgui_pfx + 'imgui_impl_opengl3.cpp',
    imgui_pfx + 'imgui_widgets.cpp',
    imgui_pfx + 'imgui_tables.cpp',
    imgui_pfx + 'imgui_draw.cpp',
    pfx + 'renderer.cpp',
    pfx + 'menu.cpp',
    pfx + 'particle_source.cpp',
    pfx + 'shader.cpp',
    pfx + 'texture.cpp',
]

glm = dependency('glm', version : '>=0.9.9')
gl = dependency('gl')
libdl = cc.find_library('dl')
glfw = dependency('glfw3')

executable('demo', sources,
include_directories : incdir,
dependencies : [
    glm,
    gl,
    libdl,
    glfw
])
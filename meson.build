project('particles', 'cpp',  default_options : ['cpp_std=c++11'])

cc = meson.get_compiler('cpp')
incdir = 'include/'
pfx = 'src/'
third_party_pfx = 'third_party/'
imgui_pfx = 'src/imgui/'

sources = [
    pfx + 'main.cpp',
    pfx + 'renderer.cpp',
    pfx + 'grid.cpp',
    pfx + 'menu.cpp',
    pfx + 'particle_source.cpp',
    pfx + 'shader.cpp',
    pfx + 'texture.cpp',
    third_party_pfx + 'glad/glad.cpp',
    third_party_pfx + 'lodepng/lodepng.cpp',
    third_party_pfx + 'imgui/imgui.cpp',
    third_party_pfx + 'imgui/imgui_impl_glfw.cpp',
    third_party_pfx + 'imgui/imgui_impl_opengl3.cpp',
    third_party_pfx + 'imgui/imgui_widgets.cpp',
    third_party_pfx + 'imgui/imgui_tables.cpp',
    third_party_pfx + 'imgui/imgui_draw.cpp',
    third_party_pfx + 'imguifiledialog/ImGuiFileDialog.cpp',
    third_party_pfx + 'imguifiledialog/CustomFont.cpp'
]

glm = dependency('glm', version : '>=0.9.9')
gl = dependency('gl')
libdl = cc.find_library('dl')
glfw = dependency('glfw3')

executable('demo', sources,
    include_directories : [
        incdir,
        third_party_pfx
    ],
    dependencies : [
        glm,
        gl,
        libdl,
        glfw
    ],
    # cpp_args: [ '-ffreestanding', '-static'],
    # link_args: ['-nostdlib', '-static', '-Wl,--entry=main'],
    # pie : false
)
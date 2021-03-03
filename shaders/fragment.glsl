#version 450 core

in vec4 color;
in vec2 fragment_uv;

out vec4 final_color;

void main() {
    final_color = color;
}
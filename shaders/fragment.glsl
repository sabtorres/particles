#version 450 core

in vec4 color;
in vec2 fragment_uv;

out vec4 final_color;
layout (location = 0) uniform sampler2D albedo_texture;

void main() {
    final_color = texture2D(albedo_texture, fragment_uv);
}
#version 450 core

in float fragment_life;
in vec2 tex_coord;

uniform float cycle;
out vec4 final_color;
layout (location = 0) uniform sampler2D albedo_texture;

void main() {
    final_color = texture2D(albedo_texture, tex_coord)
        * vec4(1.0, 1.0, 1.0, fragment_life / cycle);
}
#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float point_size;
uniform float resolution_x;
uniform float resolution_y;

in float geometry_life[];
out float fragment_life;
out vec2 tex_coord;

void send_vertices(vec4 position) {
    float x_radius = point_size / resolution_x;
    float y_radius = point_size / resolution_y;
    gl_Position = position + vec4(-x_radius, -y_radius, 0.0, 0.0);
    tex_coord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(x_radius, -y_radius, 0.0, 0.0);
    tex_coord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-x_radius, y_radius, 0.0, 0.0);
    tex_coord = vec2(0.0, 1.0);
    EmitVertex();
    gl_Position = position + vec4(x_radius, y_radius, 0.0, 0.0);
    tex_coord = vec2(1.0, 1.0);
    EmitVertex();
}

void main() {
    fragment_life = geometry_life[0];
    send_vertices(gl_in[0].gl_Position);
    EndPrimitive();
}
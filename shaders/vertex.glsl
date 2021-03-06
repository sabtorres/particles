#version 450 core

layout (location = 0) in vec3 offset;
layout (location = 1) in float life;

out vec4 color;
out vec2 fragment_uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    color = vec4(life, 0.0, 0.0, 1.0); // to do;
    fragment_uv = vec2(0.5, 0.5);
    gl_Position = projection_matrix * view_matrix
        * model_matrix * vec4(offset, 1.0);
}
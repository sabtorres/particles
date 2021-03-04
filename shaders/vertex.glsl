#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 offset;

out vec4 color;
out vec2 fragment_uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    fragment_uv = uv;
    color = vec4(1.0, 1.0, 1.0, 1.0); // to do;
    gl_Position = projection_matrix * view_matrix
        * model_matrix * vec4(position, 1.0);
}
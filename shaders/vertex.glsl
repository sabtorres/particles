#version 450 core

layout (location = 0) in vec3 offset;
layout (location = 1) in float life;

out float fragment_life;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    fragment_life = life; // to do;
    gl_Position = projection_matrix * view_matrix
        * model_matrix * vec4(offset, 1.0);
}
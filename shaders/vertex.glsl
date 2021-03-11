#version 450 core

layout (location = 0) in vec4 offset;
layout (location = 1) in float life;

out float geometry_life;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    geometry_life = life;
    gl_Position = projection_matrix * view_matrix * model_matrix * offset;
}
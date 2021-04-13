#pragma once

#include <particle_source.hpp>
#include <grid.hpp>
#include <shader.hpp>
#include <glm/glm.hpp>

const uint WINDOW_WIDTH = 1920;
const uint WINDOW_HEIGHT = 1080;

struct Renderer {
    float camera_yaw;
    float camera_pitch;
    float zoom;
    glm::vec3 camera_rotation;
    glm::vec3 camera_position;
    glm::vec3 camera_target;
    glm::vec3 camera_up;

    Renderer();
    void render(ParticleSource& source, const Shader& shader, Grid& grid, const Shader& grid_shader);  
};
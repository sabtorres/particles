#pragma once

#include <particle_source.hpp>
#include <shader.hpp>
#include <glm/glm.hpp>

struct Renderer {
    float camera_yaw;
    float camera_pitch;
    float zoom;
    glm::vec3 camera_rotation;
    glm::vec3 camera_position;
    glm::vec3 camera_target;
    glm::vec3 camera_up;

    Renderer();
    void render(ParticleSource& source, const Shader& shader, uint width, uint height);  
};
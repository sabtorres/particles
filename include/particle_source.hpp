#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float life;
};

struct ParticleSource {
    GLuint vao;
    GLuint offset_bo;
    GLuint uv_bo;
    GLuint texture_pointer;

    std::vector<Particle> particles;
    std::vector<glm::vec3> position_buffer;

    glm::vec3 position;
    glm::vec3 rotation;

    uint number_of_particles;
    float explosiveness;

    glm::vec3 initial_velocity;
    float velocity_randomness;
    glm::vec3 initial_acceleration;
    float acceleration_randomness;
    glm::vec4 color;

    ParticleSource();
    void cleanup();
    void update(double delta_time);
    void bind_buffers();
    void draw();
};
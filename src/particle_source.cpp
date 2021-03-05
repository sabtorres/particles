#include <particle_source.hpp>
const float TEST_RANDOMNESS = 0.3;

ParticleSource::ParticleSource() {
    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);

    number_of_particles = 50;
    cycle = 1.0;
    cycle_timer = cycle;
    particle_index = 0;

    initial_velocity = glm::vec3(1.0);
    velocity_randomness = TEST_RANDOMNESS;
    initial_acceleration = glm::vec3(1.0);
    acceleration_randomness = TEST_RANDOMNESS;
    color = glm::vec4(1.0, 0.0, 0.0, 1.0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for (uint i = 0; i < number_of_particles; i++) {
        position_buffer.push_back(glm::vec3(0.0));
        particles.push_back(Particle {
            glm::vec3(0.0),
            initial_velocity,
            color,
            0.0
        });
    }

    glGenBuffers(1, &offset_bo);
    glBindBuffer(GL_ARRAY_BUFFER, offset_bo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles 
        * sizeof(glm::vec3), position_buffer.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}

void ParticleSource::cleanup() {
    glDeleteBuffers(1, &this->offset_bo);
    glDeleteVertexArrays(1, &this->vao);
}

void ParticleSource::update(double delta_time) {
    double previous_timer = cycle_timer;
    cycle_timer -= delta_time;
    uint new_particles = (uint) ((previous_timer - cycle_timer)
        * number_of_particles);
    uint next_index = particle_index + new_particles;

    for (uint i = particle_index; i < next_index; i++) {
        particles[i % number_of_particles].life = cycle;
        particles[i % number_of_particles].position = glm::vec3(0.0);
        particles[i % number_of_particles].velocity = initial_velocity;
        particles[i % number_of_particles].color = color;
    }
    
    for (uint i = 0; i < number_of_particles; i++) {
        if (particles[i].life > 0.0) {
            particles[i].life -= delta_time;
            particles[i].velocity += initial_acceleration * (float)delta_time;
            particles[i].position += particles[i].velocity 
                * (float)delta_time;
        }
    }

    particle_index = next_index % number_of_particles;
}

void ParticleSource::bind_buffers() {

}

void ParticleSource::draw() {

}
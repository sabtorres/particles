#include <particle_source.hpp>
const float TEST_RANDOMNESS = 0.3;

ParticleSource::ParticleSource() {
    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);

    number_of_particles = 50;
    explosiveness = TEST_RANDOMNESS;

    initial_velocity = glm::vec3(1.0);
    velocity_randomness = TEST_RANDOMNESS;
    initial_acceleration = glm::vec3(1.0);
    acceleration_randomness = TEST_RANDOMNESS;
    color = glm::vec4(1.0, 0.0, 0.0, 1.0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for (int i = 0; i < number_of_particles; i++) {
        position_buffer.push_back(glm::vec3(0.0));
    }

    glGenBuffers(1, &offset_bo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, offset_bo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_particles 
        * sizeof(glm::vec3), position_buffer.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}

void ParticleSource::cleanup() {
    glDeleteBuffers(1, &this->offset_bo);
    glDeleteVertexArrays(1, &this->vao);
}

void ParticleSource::update(double delta_time) {

}

void ParticleSource::bind_buffers() {

}

void ParticleSource::draw() {

}
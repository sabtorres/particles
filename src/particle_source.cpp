#include <particle_source.hpp>
#include <ctime>
#include <random>
const float TEST_RANDOMNESS = 0.4;

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
        life_buffer.push_back(0.0);
        particles.push_back(Particle {
            glm::vec3(0.0),
            initial_velocity,
            initial_acceleration,
            color,
            0.0
        });
    }

    glGenBuffers(1, &offset_bo);
    glBindBuffer(GL_ARRAY_BUFFER, offset_bo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles 
        * sizeof(glm::vec3), position_buffer.data(), GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &life_bo);
    glBindBuffer(GL_ARRAY_BUFFER, life_bo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles
        * sizeof(float), life_buffer.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(0);
}

void ParticleSource::cleanup() {
    glDeleteBuffers(1, &this->offset_bo);
    glDeleteBuffers(1, &this->life_bo);
    glDeleteVertexArrays(1, &this->vao);
}

void ParticleSource::update(double delta_time) {
    double previous_timer = cycle_timer;
    cycle_timer -= delta_time;
    uint new_particles = (uint) (floor(previous_timer * number_of_particles) - 
        floor(cycle_timer * number_of_particles));
    uint next_index = particle_index + new_particles;
    float velocity_length = glm::length(initial_velocity);
    float acceleration_length = glm::length(initial_acceleration);

    for (uint i = particle_index; i < next_index; i++) {
        particles[i % number_of_particles].life = cycle;
        particles[i % number_of_particles].position = glm::vec3(0.0);
        particles[i % number_of_particles].velocity = initial_velocity
            * (1 - velocity_randomness) + velocity_randomness
            * glm::vec3(random_throw() * velocity_length,
                random_throw() * velocity_length, 
                random_throw() * velocity_length);
        particles[i % number_of_particles].acceleration = initial_acceleration
            * (1 - acceleration_randomness) + acceleration_randomness
            * glm::vec3(random_throw() * acceleration_length,
                random_throw() * acceleration_length, 
                random_throw() * acceleration_length);
        particles[i % number_of_particles].color = color;
    }
    
    for (uint i = 0; i < number_of_particles; i++) {
        if (particles[i].life > 0.0) {
            particles[i].life -= delta_time;
            particles[i].velocity += particles[i].acceleration
                * (float)delta_time;
            particles[i].position += particles[i].velocity 
                * (float)delta_time;
        }
        else {
            particles[i].life = 0.0;
            particles[i].position = glm::vec3(0.0);
            particles[i].velocity = glm::vec3(0.0);
        }

        position_buffer[i] = particles[i].position;
        life_buffer[i] = particles[i].life;
    }

    particle_index = next_index % number_of_particles;
}

void ParticleSource::bind_buffers() {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, offset_bo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
        number_of_particles * sizeof(glm::vec3), position_buffer.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, life_bo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
        number_of_particles * sizeof(float), life_buffer.data());
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
}

void ParticleSource::draw() {
    glPointSize(10.0);
    glDrawArrays(GL_POINTS, 0, number_of_particles);
}

float ParticleSource::random_throw() {
    float u = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return (u * 2.0) - 1.0;
}
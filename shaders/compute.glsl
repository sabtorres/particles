#version 450

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
layout (std430) buffer;

uniform int number_of_particles;
uniform float cycle;
uniform float explosiveness;
uniform float emission_radius;
uniform vec4 initial_velocity;
uniform float velocity_randomness;
uniform vec4 initial_acceleration;
uniform float acceleration_randomness;

uniform float delta_time;
uniform int particle_index;
uniform int new_particles;

struct Particle {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;
    float life;
    vec3 padding;
};

layout (binding = 0) buffer Particles {
    Particle all_particles[];
} particles;

void main() {
    int next_index = particle_index + new_particles;

    float velocity_length = initial_velocity.length();
    float acceleration_length = initial_acceleration.length();
    uint index = gl_GlobalInvocationID.x % number_of_particles;
    Particle p = particles.all_particles[index];

    if (index >= particle_index && index < next_index) {
        p.life = cycle;
        p.position = vec4(0.0, 0.0, 0.0, 1.0);
        p.velocity = initial_velocity;
        p.acceleration = initial_acceleration;
    }
    
    if (p.life > 0.0) {
        p.life -= delta_time;
        p.velocity += p.acceleration * delta_time;
        p.position += p.velocity * delta_time;
    }
    else {
        p.life = 0.0;
        p.position = vec4(0.0, 0.0, 0.0, 1.0);
        p.velocity = vec4(0.0);
        p.acceleration = vec4(0.0);
    }

    particles.all_particles[index] = p;
}
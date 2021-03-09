#version 450

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (binding = 0) uniform Parameters {
    int number_of_particles;
    float cycle;
    float explosiveness;
    float emission_radius;
    vec3 initial_velocity;
    float velocity_randomness;
    vec3 initial_acceleration;
    float acceleration_randomness;

    float cycle_timer;
    float delta_time;
    int particle_index;
    int particles_left;
} parameters;

layout (binding = 1) uniform Particle {
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    float life;
} particle;

void main() {
    
}
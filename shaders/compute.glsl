#version 450

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (binding = 0) uniform Parameters {
    int number_of_particles;
    float cycle;
    float explosiveness;
    float emission_radius;
    vec4 initial_velocity;
    float velocity_randomness;
    vec4 initial_acceleration;
    float acceleration_randomness;

    float delta_time;
    int particle_index;
    int new_particles;
} parameters;

struct Particle {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;
    float life;
};

layout (std430, binding = 1) coherent buffer Particles {
    Particle p[];
} particles;

void main() {
    int next_index = parameters.particle_index + parameters.new_particles;

    float velocity_length = parameters.initial_velocity.length();
    float acceleration_length = parameters.initial_acceleration.length();
    uint index = gl_LocalInvocationIndex % parameters.number_of_particles;

    if (index >= parameters.particle_index && index < next_index) {
        particles.p[index].life = parameters.cycle;
        particles.p[index].position = vec4(0.0, 0.0, 0.0, 1.0);
        particles.p[index].velocity = parameters.initial_velocity;
        particles.p[index].acceleration = parameters.initial_acceleration;
    }
    
    if (particles.p[index].life > 0.0) {
        particles.p[index].life -= parameters.delta_time;
        particles.p[index].velocity +=
            particles.p[index].acceleration * parameters.delta_time;
        particles.p[index].position +=
            particles.p[index].velocity * parameters.delta_time;
    }
    else {
        particles.p[index].life = 0.0;
        particles.p[index].position = vec4(0.0, 0.0, 0.0, 1.0);
        particles.p[index].velocity = vec4(0.0);
        particles.p[index].acceleration = vec4(0.0);
    }
}
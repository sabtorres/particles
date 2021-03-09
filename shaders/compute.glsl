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

    float delta_time;
    int particle_index;
    int new_particles;
} parameters;

struct Particle {
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    float life;
};

layout (binding = 1) coherent buffer Particles {
    Particle p[];
} particles;

void main() {
    int next_index = parameters.particle_index + parameters.new_particles;

    float velocity_length = parameters.initial_velocity.length();
    float acceleration_length = parameters.initial_acceleration.length();

    if (gl_LocalInvocationID.x >= parameters.particle_index
        && gl_LocalInvocationID.x < next_index) {
        particles.p[gl_LocalInvocationID.x].life = parameters.cycle;
        particles.p[gl_LocalInvocationID.x].position = vec3(0.0);
        particles.p[gl_LocalInvocationID.x].velocity =
            parameters.initial_velocity;
        particles.p[gl_LocalInvocationID.x].acceleration =
            parameters.initial_acceleration;
    }
    
    if (particles.p[gl_LocalInvocationID.x].life > 0.0) {
        particles.p[gl_LocalInvocationID.x].life -= parameters.delta_time;
        particles.p[gl_LocalInvocationID.x].velocity +=
            particles.p[gl_LocalInvocationID.x].acceleration
            * parameters.delta_time;
        particles.p[gl_LocalInvocationID.x].position +=
            particles.p[gl_LocalInvocationID.x].velocity 
            * parameters.delta_time;
    }
    else {
        particles.p[gl_LocalInvocationID.x].life = 0.0;
        particles.p[gl_LocalInvocationID.x].position = vec3(0.0);
        particles.p[gl_LocalInvocationID.x].velocity = vec3(0.0);
    }
}
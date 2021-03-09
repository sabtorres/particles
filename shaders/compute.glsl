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

layout (std430, binding = 1) buffer Particles {
    Particle all_particles[];
} particles;

void main() {
    int next_index = parameters.particle_index + parameters.new_particles;

    float velocity_length = parameters.initial_velocity.length();
    float acceleration_length = parameters.initial_acceleration.length();
    uint index = gl_GlobalInvocationID.x % parameters.number_of_particles;
    Particle p = particles.all_particles[index];

    if (index >= parameters.particle_index && index < next_index) {
        p.life = parameters.cycle;
        p.position = vec4(0.0, 0.0, 0.0, 1.0);
        p.velocity = parameters.initial_velocity;
        p.acceleration = parameters.initial_acceleration;
    }
    
    if (p.life > 0.0) {
        p.life -= parameters.delta_time;
        p.velocity += p.acceleration * parameters.delta_time;
        p.position += p.velocity * parameters.delta_time;
    }
    else {
        p.life = 0.0;
        p.position = vec4(0.0, 0.0, 0.0, 1.0);
        p.velocity = vec4(0.0);
        p.acceleration = vec4(0.0);
    }

    particles.all_particles[index] = p;
}
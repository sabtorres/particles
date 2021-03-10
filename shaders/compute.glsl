#version 450

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;
layout (std430) buffer;

const float M_PI = 3.1415;
const int HASH = 16;

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

uniform vec4 y_sampler;

struct Particle {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;
    float life;
};

layout (binding = 0) buffer Particles {
    Particle all_particles[];
} particles;

layout (binding = 1) uniform sampler2D noise;

vec4 spherical_to_xyz(float r, float theta, float phi) {
    return vec4(
        r * sin(theta) * sin(phi),
        r * sin(theta) * cos(phi),
        r * cos(theta),
        1.0
    );
}

float random_uniform(vec2 coordinate, float scale) {
    return texture2D(noise, coordinate).r * scale;
}

float random_throw(int index) {
    float coordinate0 = float(index) / float(number_of_particles);
    return (random_uniform(vec2(coordinate0, y_sampler.x), 1.0) * 2.0) - 1.0;
}

vec4 spawn_position(int index) {
    float coordinate0 = float(index) / float(number_of_particles);
    float r = random_uniform(vec2(coordinate0, y_sampler.y), emission_radius);
    float theta = random_uniform(vec2(coordinate0, y_sampler.z),
        2.0 * M_PI);
    float phi = random_uniform(vec2(coordinate0, y_sampler.w), M_PI);
    return spherical_to_xyz(r, theta, phi);
}

void main() {
    int next_index = particle_index + new_particles;

    float velocity_length = sqrt(initial_velocity.length());
    float acceleration_length = sqrt(initial_acceleration.length());
    int index = int(gl_GlobalInvocationID.x);
    Particle p = particles.all_particles[index];

    if (index >= particle_index && index < next_index) {
        p.life = cycle;
        p.position = spawn_position(index);
        p.velocity = initial_velocity * (1.0 - velocity_randomness)
            + vec4(random_throw(index), random_throw(index - (index % HASH)),
                random_throw(index + (index % HASH)), 0.0)
            * velocity_length * velocity_randomness;
        p.acceleration = initial_acceleration * (1.0 - acceleration_randomness)
            + vec4(random_throw(index), random_throw(index - (index % HASH)),
                random_throw(index + (index % HASH)), 0.0)
            * acceleration_length * acceleration_randomness;
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
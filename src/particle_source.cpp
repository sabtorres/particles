#include <particle_source.hpp>
#include <shader.hpp>
#include <ctime>
#include <random>
#include <cmath>

#define CLAMP(x) x >= 0.0 ? 1.0 : 0.0;
const float TEST_RANDOMNESS = 0.9;

ParticleSource::ParticleSource() {
    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);

    mode_gpu = false;
    number_of_particles = 100;
    cycle = 1.0;
    cycle_timer = cycle;
    point_size = 15.0;
    explosiveness = 0.0;
    emission_radius = 0.0;
    particle_index = 0;
    particles_left = number_of_particles;

    initial_velocity = glm::vec3(0.3);
    velocity_randomness = TEST_RANDOMNESS;
    initial_acceleration = glm::vec3(0.3);
    acceleration_randomness = TEST_RANDOMNESS;
    color = glm::vec4(1.0, 1.0, 1.0, 1.0);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &offset_bo);
    glGenBuffers(1, &life_bo);
    glGenBuffers(1, &ssbo);
    update_buffer_sizes();
    
    glBindVertexArray(vao);
    texture = Texture();
    glGenTextures(1, &texture_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width,
        texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data.data());
    
    generate_gpu_compute();
    
    glBindVertexArray(0);
}

void ParticleSource::cleanup() {
    glDeleteProgram(compute_program);
    glDeleteTextures(1, &this->texture_buffer);
    glDeleteBuffers(1, &this->offset_bo);
    glDeleteBuffers(1, &this->life_bo);
    glDeleteBuffers(1, &this->ssbo);
    glDeleteVertexArrays(1, &this->vao);
}

void ParticleSource::update(double delta_time) {
    if(!mode_gpu) {
        update_cpu(delta_time);
    }
    else {
        update_gpu(delta_time);
    }
}

void ParticleSource::update_cpu(double delta_time) {
    double previous_timer = cycle_timer;
    cycle_timer -= delta_time;

    float previous_factor = floor((previous_timer / cycle)
        * number_of_particles);
    float cycle_factor = floor((cycle_timer / cycle) 
        * number_of_particles);
    int new_particles = (int) (previous_factor - cycle_factor
        + particles_left * explosiveness);
    particles_left -= new_particles;
    int next_index = particle_index + new_particles;
    
    float velocity_length = glm::length(initial_velocity);
    float acceleration_length = glm::length(initial_acceleration);

    for (int i = particle_index; i < next_index; i++) {
        particles[i % number_of_particles].life = cycle;
        particles[i % number_of_particles].position = spawn_position();
        particles[i % number_of_particles].velocity = initial_velocity
            * (1.0f - velocity_randomness) + velocity_randomness
            * glm::vec3(random_throw() * velocity_length,
                random_throw() * velocity_length, 
                random_throw() * velocity_length);
        particles[i % number_of_particles].acceleration = initial_acceleration
            * (1.0f - acceleration_randomness) + acceleration_randomness
            * glm::vec3(random_throw() * acceleration_length,
                random_throw() * acceleration_length, 
                random_throw() * acceleration_length);
        particles[i % number_of_particles].color = color;
    }
    
    for (int i = 0; i < number_of_particles; i++) {
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

    if (cycle_timer < 0) {
        cycle_timer = cycle;
        particles_left = number_of_particles;
    }
    particle_index = next_index % number_of_particles;
}

void ParticleSource::update_gpu(double delta_time) {
    glUseProgram(compute_program);

    double previous_timer = cycle_timer;
    float previous_factor = floor((previous_timer / cycle)
        * number_of_particles);
    float cycle_factor = floor((cycle_timer / cycle) 
        * number_of_particles);
    int new_particles = (int) (previous_factor - cycle_factor
        + particles_left * explosiveness);

    send_uniform_struct(delta_time, new_particles);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    
    if (cycle_timer < 0) {
        cycle_timer = cycle;
        particles_left = number_of_particles;
    }
    particles_left -= new_particles;
    int next_index = particle_index + new_particles;
    particle_index = next_index % number_of_particles;

    glDispatchCompute(work_x, 1, 1);
    glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);

    for (int i = 0; i < number_of_particles; i++) {
        position_buffer[i] = particles[i].position;
        life_buffer[i] = particles[i].life;
    }

    glUseProgram(0);
}

void ParticleSource::send_uniform_struct(double delta_time, int new_particles) {
    auto nop_loc = glGetUniformLocation(compute_program,
        "parameters.number_of_particles");
    glUniform1i(nop_loc, number_of_particles);
    auto cycle_loc = glGetUniformLocation(compute_program,
        "parameters.cycle");
    glUniform1f(cycle_loc, cycle);
    auto explosiveness_loc = glGetUniformLocation(compute_program,
        "parameters.explosiveness");
    glUniform1f(explosiveness_loc, explosiveness);

    auto vr_loc = glGetUniformLocation(compute_program,
        "parameters.velocity_randomness");
    glUniform1f(vr_loc, velocity_randomness);
    auto ar_loc = glGetUniformLocation(compute_program,
        "parameters.acceleration_randomness");
    glUniform1f(ar_loc, acceleration_randomness);

    auto iv_loc = glGetUniformLocation(compute_program,
        "parameters.initial_velocity");
    glUniform3f(iv_loc, initial_velocity.x, initial_velocity.y,
        initial_velocity.z);
    auto ia_loc = glGetUniformLocation(compute_program,
        "parameters.initial_acceleration");
    glUniform3f(ia_loc, initial_acceleration.x, initial_acceleration.y,
        initial_acceleration.z);

    auto dt_loc = glGetUniformLocation(compute_program,
        "parameters.delta_time");
    glUniform1f(dt_loc, delta_time);
    auto pi_loc = glGetUniformLocation(compute_program,
        "parameters.particle_index");
    glUniform1i(pi_loc, particle_index);
    auto np_loc = glGetUniformLocation(compute_program,
        "parameters.new_particles");
    glUniform1i(np_loc, new_particles);
}

void ParticleSource::update_buffer_sizes() {
    particles.clear();
    position_buffer.clear();
    life_buffer.clear();

    for (int i = 0; i < number_of_particles; i++) {
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

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, offset_bo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles 
        * sizeof(glm::vec3), position_buffer.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, life_bo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles
        * sizeof(float), life_buffer.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, number_of_particles
        * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void ParticleSource::generate_gpu_compute() {
    int work_grp_size[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
    work_x = work_grp_size[0];
    work_y = work_grp_size[1];
    work_z = work_grp_size[2];
    
    GLuint compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    std::string shader_source = 
        Shader::load_shader_string("../shaders/compute.glsl");
    const char* char_pointer_source = shader_source.c_str();
    int length = shader_source.length();
    glShaderSource(compute_shader, 1, &char_pointer_source, &length);
    glCompileShader(compute_shader);
    Shader::check_shader_error(compute_shader, GL_COMPILE_STATUS, false);
    compute_program = glCreateProgram();
    glAttachShader(compute_program, compute_shader);
    glLinkProgram(compute_program);
    Shader::check_shader_error(compute_program, GL_LINK_STATUS, true);
    glValidateProgram(compute_program);
    Shader::check_shader_error(compute_program, GL_VALIDATE_STATUS, true);

    glDetachShader(compute_program, compute_shader);
    glDeleteShader(compute_shader);
}

void ParticleSource::bind_buffers() {
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_buffer);

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
    glPointSize(point_size);
    glDrawArrays(GL_POINTS, 0, number_of_particles);
}

float ParticleSource::random_uniform(float scale) {
    return (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX))
        * scale;
}

float ParticleSource::random_throw() {
    return (random_uniform(1.0) * 2.0) - 1.0;
}

glm::vec3 ParticleSource::spawn_position() {
    float r = random_uniform(emission_radius);
    float theta = random_uniform(2.0 * M_PI);
    float phi = random_uniform(M_PI);
    return spherical_to_xyz(r, theta, phi);
}

glm::vec3 ParticleSource::spherical_to_xyz(float r, float theta, float phi) {
    return glm::vec3(
        r * sinf(theta) * sinf(phi),
        r * sinf(theta) * cosf(phi),
        r * cosf(theta)
    );
}
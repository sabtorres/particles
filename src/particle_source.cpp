#include <particle_source.hpp>
#include <shader.hpp>
#include <random>
#include <cmath>
#include <cstring>
#include <cstdlib>

const float TEST_RANDOMNESS = 0.9;
const int LOCAL_GROUPS = 64;

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

    initial_velocity = glm::vec4(0.3, 0.3, 0.3, 0.0);
    velocity_randomness = TEST_RANDOMNESS;
    initial_acceleration = glm::vec4(0.3, 0.3, 0.3, 0.0);
    acceleration_randomness = TEST_RANDOMNESS;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ssbo);
    update_buffer_sizes();
    
    glBindVertexArray(vao);
    texture = Texture("../resources/tex1.png");
    setup_texture(texture, texture_buffer, GL_RGBA, GL_TEXTURE_2D);
    noise = Texture();
    setup_texture(noise, noise_buffer, GL_RED, GL_TEXTURE_2D);
    
    generate_gpu_compute();
    
    glBindVertexArray(0);
}

void ParticleSource::cleanup() {
    glDeleteProgram(compute_program);
    glDeleteTextures(1, &this->texture_buffer);
    glDeleteBuffers(1, &this->ssbo);
    glDeleteVertexArrays(1, &this->vao);
}

void ParticleSource::update(double delta_time) {
    if(!mode_gpu) {
        update_cpu(delta_time);
    }
    else if (mode_gpu) {
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
    
    float velocity_length = sqrt(initial_velocity.length());
    float acceleration_length = sqrt(initial_acceleration.length());

    for (int i = particle_index; i < next_index; i++) {
        particles[i % number_of_particles].life = cycle;
        particles[i % number_of_particles].position = spawn_position();
        particles[i % number_of_particles].velocity = initial_velocity
            * (1.0f - velocity_randomness) + velocity_randomness
            * glm::vec4(random_throw(), random_throw(), random_throw(), 0.0)
            * velocity_length;
        particles[i % number_of_particles].acceleration = initial_acceleration
            * (1.0f - acceleration_randomness) + acceleration_randomness
            * glm::vec4(random_throw(), random_throw(), random_throw(), 0.0)
            * acceleration_length;
    }
    
    for (int i = 0; i < number_of_particles; i++) {
        if (particles[i].life > 0.0) {
            particles[i].life -= delta_time;
            particles[i].velocity += particles[i].acceleration
                * (float) delta_time;
            particles[i].position += particles[i].velocity 
                * (float) delta_time;
        }
        else {
            particles[i].life = 0.0;
            particles[i].position = glm::vec4(0.0, 0.0, 0.0, 1.0);
            particles[i].velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
            particles[i].acceleration = glm::vec4(0.0, 0.0, 0.0, 0.0);
        }
    }

    if (cycle_timer < 0) {
        cycle_timer = cycle;
        particles_left = number_of_particles;
    }
    particle_index = next_index % number_of_particles;
}

void ParticleSource::update_gpu(double delta_time) {
    double previous_timer = cycle_timer;
    cycle_timer -= delta_time;
    float previous_factor = floor((previous_timer / cycle)
        * number_of_particles);
    float cycle_factor = floor((cycle_timer / cycle) 
        * number_of_particles);
    int new_particles = (int) (previous_factor - cycle_factor
        + particles_left * explosiveness);

    glUseProgram(compute_program);
    send_uniform_struct(delta_time, new_particles);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    glDispatchCompute((number_of_particles / LOCAL_GROUPS) + 1, 1, 1);
    if (cycle_timer < 0.0) {
        cycle_timer = cycle;
        particles_left = number_of_particles;
    }
    particles_left -= new_particles;
    int next_index = particle_index + new_particles;
    particle_index = next_index % number_of_particles;
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glUseProgram(0);
}

void ParticleSource::send_uniform_struct(double delta_time, int new_particles) {
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, noise_buffer);
    
    auto nop_loc = glGetUniformLocation(compute_program,
        "number_of_particles");
    glUniform1i(nop_loc, number_of_particles);
    auto cycle_loc = glGetUniformLocation(compute_program,
        "cycle");
    glUniform1f(cycle_loc, cycle);
    auto explosiveness_loc = glGetUniformLocation(compute_program,
        "explosiveness");
    glUniform1f(explosiveness_loc, explosiveness);
    auto radius_loc = glGetUniformLocation(compute_program,
        "emission_radius");
    glUniform1f(radius_loc, emission_radius);

    auto vr_loc = glGetUniformLocation(compute_program,
        "velocity_randomness");
    glUniform1f(vr_loc, velocity_randomness);
    auto ar_loc = glGetUniformLocation(compute_program,
        "acceleration_randomness");
    glUniform1f(ar_loc, acceleration_randomness);

    auto iv_loc = glGetUniformLocation(compute_program,
        "initial_velocity");
    glUniform4f(iv_loc, initial_velocity.x, initial_velocity.y,
        initial_velocity.z, initial_velocity.w);
    auto ia_loc = glGetUniformLocation(compute_program,
        "initial_acceleration");
    glUniform4f(ia_loc, initial_acceleration.x, initial_acceleration.y,
        initial_acceleration.z, initial_acceleration.w);

    auto dt_loc = glGetUniformLocation(compute_program,
        "delta_time");
    glUniform1f(dt_loc, (float) delta_time);
    auto pi_loc = glGetUniformLocation(compute_program,
        "particle_index");
    glUniform1i(pi_loc, particle_index);
    auto np_loc = glGetUniformLocation(compute_program,
        "new_particles");
    glUniform1i(np_loc, new_particles);

    auto ys_loc = glGetUniformLocation(compute_program,
        "y_sampler");
    glUniform4f(ys_loc, random_uniform(1.0), random_uniform(1.0),
        random_uniform(1.0), random_uniform(1.0));
}

void ParticleSource::update_buffer_sizes() {
    particles.clear();

    for (int i = 0; i < number_of_particles; i++) {
        particles.push_back(Particle {
            glm::vec4(0.0, 0.0, 0.0, 1.0),
            initial_velocity,
            initial_acceleration,
            0.0,
            glm::vec3(0.0)
        });
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, ssbo);
    glBufferData(GL_ARRAY_BUFFER, number_of_particles
        * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void ParticleSource::generate_gpu_compute() {
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_x);
    
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

void ParticleSource::setup_texture(const Texture& m_texture,
    GLuint& buffer, GLint flag, GLint dimension) {
    glGenTextures(1, &buffer);
	glBindTexture(dimension, buffer);

	glTexParameteri(dimension, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(dimension, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(dimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(dimension, 0, flag, m_texture.width,
        m_texture.height, 0, flag, GL_UNSIGNED_BYTE, m_texture.data.data());
}

void ParticleSource::bind_buffers() {
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, ssbo);
    if (!mode_gpu) {
        glBufferSubData(GL_ARRAY_BUFFER, 0,
            number_of_particles * sizeof(Particle), particles.data());
    }
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), nullptr);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
        (GLvoid*) 48);
    glEnableVertexAttribArray(0);
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

glm::vec4 ParticleSource::spawn_position() {
    float r = random_uniform(emission_radius);
    float theta = random_uniform(2.0 * M_PI);
    float phi = random_uniform(M_PI);
    return spherical_to_xyz(r, theta, phi);
}

glm::vec4 ParticleSource::spherical_to_xyz(float r, float theta, float phi) {
    return glm::vec4(
        r * sinf(theta) * sinf(phi),
        r * sinf(theta) * cosf(phi),
        r * cosf(theta),
        1.0
    );
}
#include <vector>
#include <glm/glm.hpp>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float life;
};

struct ParticleSource {
    std::vector<Particle> particles;
    glm::vec3 position;
    glm::vec3 rotation;

    glm::vec3 initial_velocity;
    glm::vec3 initial_acceleration;


    ParticleSource();
    void cleanup();
    void update(double delta_time);
    void bind_buffers();
    void draw();
};
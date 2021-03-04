#include <renderer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void render(ParticleSource& source, const Shader& shader) {
    auto translate = glm::identity<glm::mat4>();
    auto rotation = glm::identity<glm::mat4>();
    auto scale = glm::identity<glm::mat4>();

    auto model_mat = translate * scale * rotation;

    auto camera_eye = glm::vec3(0.0, 0.0, 3.0);
    auto view_mat = glm::lookAt(
        camera_eye,
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0)
    );

    auto projection_mat = glm::perspective<float>(45.0, 16.0/9.0, 0.001, 1000);

    glUseProgram(shader.program);

    auto projection_loc = glGetUniformLocation(shader.program, "projection_matrix");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection_mat[0][0]);

    auto view_loc = glGetUniformLocation(shader.program, "view_matrix");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_mat[0][0]);

    auto model_loc = glGetUniformLocation(shader.program, "model_matrix");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model_mat[0][0]);

    source.bind_buffers();
    source.draw();

    glUseProgram(0);
}
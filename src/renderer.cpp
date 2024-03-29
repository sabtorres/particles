#include <renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() {
    camera_yaw = 45.0;
    camera_pitch = 45.0;
    zoom = 3.0;
    glm::vec3 direction;
    direction.x = cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
    direction.y = sin(glm::radians(camera_pitch));
    direction.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
    camera_position = glm::normalize(direction);
    camera_target = glm::vec3(0.0, 0.0, 0.0);
    camera_up = glm::vec3(0.0, 1.0, 0.0);
}

void Renderer::render(ParticleSource& source, const Shader& shader, Grid& grid, const Shader& grid_shader) {
    auto model_mat = glm::identity<glm::mat4>();

    auto real_position = camera_position * zoom;

    auto view_mat = glm::lookAt(
        real_position,
        camera_target,
        camera_up
    );

    auto projection_mat = glm::perspective<float>(45.0, 16.0/9.0, 0.001, 1000);

    glUseProgram(grid_shader.program);
    auto projection_loc = glGetUniformLocation(grid_shader.program, "projection_matrix");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection_mat[0][0]);
    auto view_loc = glGetUniformLocation(grid_shader.program, "view_matrix");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_mat[0][0]);
    grid.render();

    glUseProgram(shader.program);
    projection_loc = glGetUniformLocation(shader.program, "projection_matrix");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection_mat[0][0]);
    view_loc = glGetUniformLocation(shader.program, "view_matrix");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_mat[0][0]);
    auto model_loc = glGetUniformLocation(shader.program, "model_matrix");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model_mat[0][0]);
    auto cycle_loc = glGetUniformLocation(shader.program, "cycle");
    glUniform1f(cycle_loc, source.cycle);
    auto size_loc = glGetUniformLocation(shader.program, "point_size");
    glUniform1f(size_loc, source.point_size);
    auto width_loc = glGetUniformLocation(shader.program, "resolution_x");
    glUniform1f(width_loc, WINDOW_WIDTH);
    auto height_loc = glGetUniformLocation(shader.program, "resolution_y");
    glUniform1f(height_loc, WINDOW_HEIGHT);
    source.bind_buffers();
    source.draw();

    glUseProgram(0);
}
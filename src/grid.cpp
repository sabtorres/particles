#include <grid.hpp>
#include <vector>
#include <glm/glm.hpp>

Grid::Grid() {
    std::vector<glm::vec3> positions;
    std::vector<int> indices;
    index_count = 0;
    for(float x = -5.0; x < 6.0; x += 1.0) {
        positions.push_back(glm::vec3(x, 0.0, -5.0));
        indices.push_back(index_count);
        index_count++;
        positions.push_back(glm::vec3(x, 0.0, 5.0));
        indices.push_back(index_count);
        index_count++;
    }

    for(float z = -5.0; z < 6.0; z += 1.0) {
        positions.push_back(glm::vec3(-5.0, 0.0, z));
        indices.push_back(index_count);
        index_count++;
        positions.push_back(glm::vec3(5.0, 0.0, z));
        indices.push_back(index_count);
        index_count++;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * index_count * sizeof(int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Grid::render() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_LINES, index_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Grid::cleanup() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}
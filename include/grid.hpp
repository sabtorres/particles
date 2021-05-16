#pragma once

#include <glad/glad.h>

struct Grid {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    int index_count;

    Grid();
    void render();
    void cleanup();
};
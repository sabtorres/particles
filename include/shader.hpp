#pragma once

#include <glad/glad.h>
#include <string>

const std::string VERTEX_FILE = "../shaders/vertex.glsl";
const std::string FRAGMENT_FILE = "../shaders/fragment.glsl";

struct Shader {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;

    Shader();
    void cleanup();
    std::string load_shader_string(const std::string& filename);
    GLint check_shader_error(GLuint shader, GLuint flag, bool is_program);
};
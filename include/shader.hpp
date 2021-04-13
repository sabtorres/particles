#pragma once

#include <glad/glad.h>
#include <string>

const std::string VERTEX_FILE = "../shaders/vertex.glsl";
const std::string FRAGMENT_FILE = "../shaders/fragment.glsl";
const std::string GEOMETRY_FILE = "../shaders/geometry.glsl";

struct Shader {
    GLuint program;
    GLuint vertex_shader;
    GLuint geometry_shader;
    GLuint fragment_shader;

    Shader();
    Shader(const std::string& vs_file, const std::string& fs_file);
    void setup_program(const std::string& vertex_shader_source, const std::string& geometry_shader_source, const std::string& fragment_shader_source);
    void cleanup();
    static std::string load_shader_string(const std::string& filename);
    static GLint check_shader_error(GLuint shader, GLuint flag, bool is_program);
};
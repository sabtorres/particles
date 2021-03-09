#include <shader.hpp>
#include <iostream>
#include <fstream>

Shader::Shader() {
    program = glCreateProgram();

    if(program == 0) {
        std::cerr<<"Couldn't find a valid address in memory to build the program\n";
    }

    std::string vertex_shader_source = load_shader_string(VERTEX_FILE);
    std::string fragment_shader_source = load_shader_string(FRAGMENT_FILE);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* char_pointer_source = vertex_shader_source.c_str();
    int length_pointer = vertex_shader_source.length();
    glShaderSource(vertex_shader, 1, &char_pointer_source, &length_pointer);
    glCompileShader(vertex_shader);
    check_shader_error(vertex_shader, GL_COMPILE_STATUS, false);
    glAttachShader(program, vertex_shader);

    char_pointer_source = fragment_shader_source.c_str();
    length_pointer = fragment_shader_source.length();
    glShaderSource(fragment_shader, 1, &char_pointer_source, &length_pointer);
    glCompileShader(fragment_shader);
    check_shader_error(fragment_shader, GL_COMPILE_STATUS, false);
    glAttachShader(program, fragment_shader);


    glLinkProgram(program);
    check_shader_error(program, GL_LINK_STATUS, true);

    glValidateProgram(program);
    check_shader_error(program, GL_VALIDATE_STATUS, true);

    glDetachShader(this->program, this->vertex_shader);
    glDeleteShader(this->vertex_shader);
    glDetachShader(this->program, this->fragment_shader);
    glDeleteShader(this->fragment_shader);
}

std::string Shader::load_shader_string(const std::string& filename) {
	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if(!file.is_open()) {
        return "";
	}

    while(file.good()) {
        std::getline(file, line);
        output.append(line + "\n");
    }

	return output;
}

GLint Shader::check_shader_error(GLuint shader, GLuint flag, bool is_program) {
	GLint success = 0;
	GLchar error[1024] = {0};

	if(is_program)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if(success == GL_FALSE) {
		if(is_program)
			glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), nullptr, error);

        printf("Error: %s\n", (char*)error);
		throw std::runtime_error("shader stage failed!");
	}

    return success;
}

void Shader::cleanup() {
    glDeleteProgram(this->program);
}
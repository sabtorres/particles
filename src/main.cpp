#include <cstdio>
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <particle_source.hpp>
#include <shader.hpp>
#include <renderer.hpp>
#include <menu.hpp>

const uint WINDOW_WIDTH = 1920;
const uint WINDOW_HEIGHT = 1080;
double last_x, last_y;
Renderer renderer;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void get_camera_input(GLFWwindow* window, double x_pos, double y_pos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Particles", nullptr, nullptr);

    if(window == nullptr) {
        std::cout<<"Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwGetCursorPos(window, &last_x, &last_y);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout<<"Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // initialize
    Menu menu = Menu(window);
    Shader shader = Shader();
    ParticleSource source = ParticleSource();
    renderer = Renderer();

    // render loop
    double delta_time = 0;
    auto begin_time = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        source.update(delta_time);
        renderer.render(source, shader, WINDOW_WIDTH, WINDOW_HEIGHT);
        menu.render(source, delta_time);
        glfwSwapBuffers(window);

        auto end_time = std::chrono::high_resolution_clock::now();
        delta_time = std::chrono::duration<double>(
            (end_time - begin_time)).count();
        begin_time = std::chrono::high_resolution_clock::now();
    }
    
    menu.cleanup();
    source.cleanup();
    shader.cleanup();
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void get_camera_input(GLFWwindow* window, double x_pos, double y_pos) {
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos; 
    last_x = x_pos;
    last_y = y_pos;

    float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    renderer.camera_yaw += x_offset;
    renderer.camera_pitch += y_offset;

    if(renderer.camera_pitch > 89.0f)
        renderer.camera_pitch = 89.0f;
    if(renderer.camera_pitch < -89.0f)
        renderer.camera_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(renderer.camera_yaw)) * cos(glm::radians(renderer.camera_pitch));
    direction.y = sin(glm::radians(renderer.camera_pitch));
    direction.z = sin(glm::radians(renderer.camera_yaw)) * cos(glm::radians(renderer.camera_pitch));
    renderer.camera_position = glm::normalize(direction);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, get_camera_input);
    }
    
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
    }
}
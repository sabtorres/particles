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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    // render loop
    double delta_time = 0;
    auto begin_time = std::chrono::high_resolution_clock::now();
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        source.update(delta_time);
        render(source, shader);
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
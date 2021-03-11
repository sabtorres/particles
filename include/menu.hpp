#pragma once

#include <imgui/imgui.h>
#include <particle_source.hpp>
#include <GLFW/glfw3.h>

struct Menu {
    bool show_texture_window;
    ImVec4 clear_color;

    Menu(GLFWwindow* window);
    void render(ParticleSource& source, double delta_time);
    void cleanup();
};
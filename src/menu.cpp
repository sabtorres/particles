#include <menu.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Menu::Menu(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize = ImVec2(400.0, 200.0);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    show_demo_window = true;
}

void Menu::render(ParticleSource& source) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Menu");
    if(show_demo_window) {
        ImGui::SliderFloat("Initial velocity X",
        &source.initial_velocity.x, 0.0f, 10.0f);
        ImGui::SliderFloat("Initial velocity Y",
        &source.initial_velocity.y, 0.0f, 10.0f);
        ImGui::SliderFloat("Initial velocity Z",
        &source.initial_velocity.z, 0.0f, 10.0f);

        ImGui::SliderFloat("Initial acceleration X",
            &source.initial_acceleration.x, 0.0f, 10.0f);
        ImGui::SliderFloat("Initial acceleration Y",
            &source.initial_acceleration.y, 0.0f, 10.0f);
        ImGui::SliderFloat("Initial acceleration Z",
            &source.initial_acceleration.z, 0.0f, 10.0f);
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
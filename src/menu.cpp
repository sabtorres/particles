#include <menu.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

Menu::Menu(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
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
        ImGui::SliderFloat("Cycle",
            &source.cycle, 0.0f, 10.0f);
        if (ImGui::SliderInt("Number of particles",
            &source.number_of_particles, 1, 9999)) {
                source.update_buffer_sizes();
            }
        ImGui::SliderFloat("Point size",
            &source.point_size, 1.0f, 100.0f);
        ImGui::SliderFloat("Explosiveness",
            &source.explosiveness, 0.0f, 1.0f);
        ImGui::SliderFloat("Emission radius",
            &source.emission_radius, 0.0f, 10.0f);

        ImGui::SliderFloat("Velocity randomness",
            &source.velocity_randomness, 0.0f, 1.0f);
        ImGui::SliderFloat("Initial velocity X",
            &source.initial_velocity.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Initial velocity Y",
            &source.initial_velocity.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Initial velocity Z",
            &source.initial_velocity.z, -10.0f, 10.0f);

        ImGui::SliderFloat("Acceleration randomness",
            &source.acceleration_randomness, 0.0f, 1.0f);
        ImGui::SliderFloat("Initial acceleration X",
            &source.initial_acceleration.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Initial acceleration Y",
            &source.initial_acceleration.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Initial acceleration Z",
            &source.initial_acceleration.z, -10.0f, 10.0f);
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
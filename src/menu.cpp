#include <menu.hpp>
#include <texture.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imguifiledialog/ImGuiFileDialog.h>

Menu::Menu(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    show_texture_window = false;
}

void Menu::render(ParticleSource& source, double delta_time) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Menu");
    ImGui::Text("seconds per frame: %f", delta_time);
    ImGui::Checkbox("GPU compute", &source.mode_gpu);

    if (ImGui::Button("Change texture", ImVec2(200.0, 25.0))) {
        show_texture_window = true;
    }

    if (show_texture_window) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseFileDlgKey", "Choose File", ".png", "./../resources/"
        );

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                source.texture = Texture(
                    ImGuiFileDialog::Instance()->GetFilePathName());
                source.setup_texture(source.texture, source.texture_buffer,
                    GL_RGBA, GL_TEXTURE_2D);
                show_texture_window = false;
            }
            
            ImGuiFileDialog::Instance()->Close();
        }
    }

    ImGui::SliderFloat("Cycle",
        &source.cycle, 0.0f, 10.0f);
    if (ImGui::SliderInt("Number of particles",
        &source.number_of_particles, 1, 9999)) {
            source.update_buffer_sizes();
        }
    ImGui::SliderFloat("Point size",
        &source.point_size, 1.0f, 256.0f);
    ImGui::SliderFloat("Explosiveness",
        &source.explosiveness, 0.0f, 1.0f);
    ImGui::SliderFloat("Emission radius",
        &source.emission_radius, 0.0f, 10.0f);

    ImGui::SliderFloat("Velocity randomness",
        &source.velocity_randomness, 0.0f, 1.0f);
    ImGui::SliderFloat("Initial velocity X",
        &source.initial_velocity.x, -5.0f, 5.0f);
    ImGui::SliderFloat("Initial velocity Y",
        &source.initial_velocity.y, -5.0f, 5.0f);
    ImGui::SliderFloat("Initial velocity Z",
        &source.initial_velocity.z, -5.0f, 5.0f);

    ImGui::SliderFloat("Acceleration randomness",
        &source.acceleration_randomness, 0.0f, 1.0f);
    ImGui::SliderFloat("Initial acceleration X",
        &source.initial_acceleration.x, -5.0f, 5.0f);
    ImGui::SliderFloat("Initial acceleration Y",
        &source.initial_acceleration.y, -5.0f, 5.0f);
    ImGui::SliderFloat("Initial acceleration Z",
        &source.initial_acceleration.z, -5.0f, 5.0f);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
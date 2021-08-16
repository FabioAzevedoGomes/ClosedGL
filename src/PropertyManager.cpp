#include "PropertyManager.hpp"

PropertyManager::PropertyManager(GLFWwindow *window)
{
    this->window = window;
    this->showWindow = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
}

void PropertyManager::RenderWindow()
{
    if (showWindow)
    {
        ImGui::Begin("Properties", &showWindow, ImGuiWindowFlags_MenuBar);

        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::Text("Rotation");
            ImGui::SliderFloat("Pitch", (float *)&properties.rotationPitch, -2.0f * M_PI, 2.0f * M_PI);
            ImGui::SliderFloat("Roll", (float *)&properties.rotationRoll, -2.0f * M_PI, 2.0f * M_PI);
            ImGui::SliderFloat("Yaw", (float *)&properties.rotationYaw, -2.0f * M_PI, 2.0f * M_PI);

            ImGui::Text("Movement");
            // TODO

            if (ImGui::Button("Reset"))
                properties.resetCamera = true;
        }

        if (ImGui::CollapsingHeader("Model"))
        {
            ImGui::Text("Uniform Color");
            ImGui::ColorEdit3("", (float *)&properties.modelColor);

            ImGui::Text("Rendering Mode");
            ImGui::RadioButton("Filled", &properties.renderMode, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Wireframe", &properties.renderMode, 1);

            ImGui::Text("Model source");
            if (ImGui::Button("Select..."))
                ImGui::OpenPopup("Open File");
            if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".in"))
            {
                properties.reloadFile = true;
                properties.modelFilePath = fileDialog.selected_path;
            }

            ImGui::Text("Orientation");
            // TODO
        }

        if (ImGui::CollapsingHeader("Projection"))
        {
            ImGui::Text("Field of View");
            ImGui::SliderFloat("FoV", (float *)&properties.fieldOfView, 0, M_PI);

            ImGui::Text("Planes");
            ImGui::SliderFloat("Near", (float *)&properties.nearPlane, 0.1f, 10000.0f);
            ImGui::SliderFloat("Far", (float *)&properties.farPlane, 0.1f, 10000.0f);
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void PropertyManager::AdvanceFrame()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderWindow();
}

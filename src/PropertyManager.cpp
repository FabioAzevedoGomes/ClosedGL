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

        ImGui::Text("Model Color");
        ImGui::ColorEdit3("clear color", (float *)&properties.modelColor);

        ImGui::Text("Camera");
        ImGui::SliderFloat("Pitch", (float *)&properties.rotationPitch, -2.0f * M_PI, 2.0f * M_PI);
        ImGui::SliderFloat("Roll", (float *)&properties.rotationRoll, -2.0f * M_PI, 2.0f * M_PI);
        ImGui::SliderFloat("Yaw", (float *)&properties.rotationYaw, -2.0f * M_PI, 2.0f * M_PI);

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

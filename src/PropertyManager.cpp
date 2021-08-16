#include "PropertyManager.hpp"

PropertyManager::PropertyManager(GLFWwindow *window)
{
    this->window = window;

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
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::Text("Rotation");
        ImGui::SliderFloat("Pitch", (float *)&properties.rotationPitch, -2.0f * M_PI, 2.0f * M_PI);
        ImGui::SliderFloat("Roll", (float *)&properties.rotationRoll, -2.0f * M_PI, 2.0f * M_PI);
        ImGui::SliderFloat("Yaw", (float *)&properties.rotationYaw, -2.0f * M_PI, 2.0f * M_PI);

        ImGui::Text("Movement");
        ImGui::Text("  ");
        ImGui::SameLine();
        if (ImGui::Button("^"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Forward;
        }
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        if (ImGui::Button("Up"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Up;
        }

        if (ImGui::Button("<"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Left;
        }
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        if (ImGui::Button(">"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Right;
        }

        ImGui::Text("  ");
        ImGui::SameLine();
        if (ImGui::Button("v"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Backwards;
        }
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        if (ImGui::Button("Down"))
        {
            properties.shouldMove = true;
            properties.movementDirection = Down;
        }

        ImGui::SliderFloat("Speed", (float *)&properties.speed, 0.1f, 100.0f);
        ImGui::Checkbox("Look at Model", &properties.keepLookingAtModel);

        ImGui::Text("Reset Position");
        if (ImGui::Button("Reset"))
            properties.resetCamera = true;

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Model"))
    {
        ImGui::Text("Uniform Color");
        ImGui::ColorEdit3("##Uniform Color Edit", (float *)&properties.modelColor);

        ImGui::Text("Model source");
        if (ImGui::Button("Select..."))
            ImGui::OpenPopup("Open File");
        if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".in"))
        {
            properties.reloadFile = true;
            properties.modelFilePath = fileDialog.selected_path;
        }

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Rendering"))
    {
        ImGui::Text("Rendering Mode");
        ImGui::RadioButton("Filled", &properties.renderMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Wireframe", &properties.renderMode, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Points", &properties.renderMode, 2);

        ImGui::Text("Normal Orientation");
        ImGui::RadioButton("Clockwise", &properties.normalOrientation, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Counter-Clockwise", &properties.normalOrientation, 1);

        ImGui::Text("Culling");
        ImGui::RadioButton("None", &properties.cullingMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Backface", &properties.cullingMode, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Frontface", &properties.cullingMode, 2);
    }

    if (ImGui::CollapsingHeader("Projection"))
    {
        ImGui::Text("Field of View");
        ImGui::SliderFloat("FoV", (float *)&properties.fieldOfView, 0, M_PI);

        ImGui::Text("Planes");
        ImGui::SliderFloat("Near", (float *)&properties.nearPlane, 0.1f, 10000.0f);
        ImGui::SliderFloat("Far", (float *)&properties.farPlane, 0.1f, 10000.0f);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Ambient"))
    {
        ImGui::Text("Background color");
        ImGui::ColorEdit3("##Background Color Edit", (float *)&properties.backgroundColor);

        ImGui::Spacing();
    }

    ImGui::End();

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

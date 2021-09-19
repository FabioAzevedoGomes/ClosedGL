#include "PropertyManager.hpp"

PropertyManager::PropertyManager(GLFWwindow *window, Scene *scene, RenderingManager *renderingManager)
{
    this->modelLoaded = false;

    this->managedRenderer = renderingManager;
    this->managedScene = scene;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
}

PropertyManager::~PropertyManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PropertyManager::RenderWelcomeWindow()
{
    ImGui::Begin("Welcome", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("Select Model ..."))
        ImGui::OpenPopup("Open File");
    if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".in"))
        LoadInputFile(fileDialog.selected_path.c_str());
    ImGui::End();
}

void PropertyManager::RenderPropertyWindow()
{
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::RadioButton("OpenGL", &properties.engine, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Close2GL", &properties.engine, 1);
    ImGui::Spacing();

    if (ImGui::Button("Select Model..."))
        ImGui::OpenPopup("Open File");
    if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".in"))
        LoadInputFile(fileDialog.selected_path.c_str());

    ImGui::Spacing();

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

        ImGui::SliderFloat("Speed", (float *)&properties.speed, 0.1f, 1000.0f);
        ImGui::Checkbox("Look at Model", &properties.keepLookingAtModel);

        ImGui::Text("Reset Position");
        if (ImGui::Button("Reset"))
            properties.resetCamera = true;

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
        ImGui::RadioButton("Clockwise", &properties.orientation, -1);
        ImGui::SameLine();
        ImGui::RadioButton("Counter-Clockwise", &properties.orientation, 1);

        ImGui::Text("Culling");
        ImGui::RadioButton("None", &properties.cullingMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Backface", &properties.cullingMode, -1);
        ImGui::SameLine();
        ImGui::RadioButton("Frontface", &properties.cullingMode, 1);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Projection"))
    {
        ImGui::Text("Horizontal Field of View");
        ImGui::SliderFloat("HFoV", (float *)&(managedScene->camera).horizontalFieldOfView, -M_PI, M_PI);
        ImGui::Spacing();
        ImGui::Text("Vertical Field of View");
        ImGui::SliderFloat("VFoV", (float *)&(managedScene->camera).verticalFieldOfView, -M_PI, M_PI);
        ImGui::Spacing();

        ImGui::Text("Planes");
        ImGui::SliderFloat("Near", (float *)&(managedScene->camera).nearPlane, 0.1f, 10000.0f);
        ImGui::SliderFloat("Far", (float *)&(managedScene->camera).farPlane, 0.1f, 10000.0f);

        ImGui::Spacing();
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        ImGui::Checkbox("Enable light", &properties.lightOn);
        ImGui::Spacing();

        ImGui::Text("Diffuse Color");
        ImGui::ColorEdit3("##Diffuse Color Edit", (float *)&properties.modelDiffuseColor);
        ImGui::SameLine();
        ImGui::SliderFloat("Intensity ##DIFFUSE", (float *)&properties.diffuseIntensity, 0.0f, 1.0f);
        ImGui::Spacing();

        ImGui::Text("Ambient Color");
        ImGui::ColorEdit3("##Ambient Color Edit", (float *)&properties.modelAmbientColor);
        ImGui::SameLine();
        ImGui::SliderFloat("Intensity ##AMBIENT", (float *)&properties.ambientIntensity, 0.0f, 1.0f);
        ImGui::Spacing();

        ImGui::Text("Specular Color");
        ImGui::ColorEdit3("##Specular Color Edit", (float *)&properties.modelSpecularColor);
        ImGui::SameLine();
        ImGui::SliderFloat("Intensity ##SPECULAR", (float *)&properties.specularIntensity, 0.0f, 1.0f);
        ImGui::Spacing();

        ImGui::Text("Shine Coefficient");
        ImGui::SliderFloat("##Shine Coefficient", (float *)&properties.modelShineCoefficient, 0.0, 100.0);

        ImGui::Text("Lighting Mode");
        ImGui::RadioButton("Flat", &properties.lightingMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Gouraud AD", &properties.lightingMode, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Gouraud ADS", &properties.lightingMode, 2);
        ImGui::SameLine();
        ImGui::RadioButton("Phong", &properties.lightingMode, 3);
        ImGui::Spacing();

        ImGui::Text("Background color");
        ImGui::ColorEdit3("##Background Color Edit", (float *)&properties.backgroundColor);
        ImGui::Spacing();
    }

    ImGui::End();
}

void PropertyManager::RenderWindow()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (!modelLoaded)
        RenderWelcomeWindow();
    else
        RenderPropertyWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PropertyManager::ApplyPropertiesToScene()
{
    if (properties.shouldMove)
    {
        managedScene->camera.MoveTo(properties.movementDirection, properties.speed);

        if (properties.keepLookingAtModel)
            managedScene->camera.LookAtFramedObject();

        properties.shouldMove = false;
    }

    if (properties.resetCamera)
    {
        managedScene->ResetCamera();

        properties.rotationPitch = 0.0f;
        properties.rotationRoll = 0.0f;
        properties.rotationYaw = 0.0f;

        properties.resetCamera = false;
    }

    if (!properties.keepLookingAtModel)
        managedScene->camera.Rotate(properties.rotationPitch, properties.rotationRoll, properties.rotationYaw);
    else
        managedScene->camera.LookAtFramedObject();
}

void PropertyManager::ApplyPropertiesToRenderingEngine()
{
    managedRenderer->SelectEngine(Engines(properties.engine), *managedScene);
    managedRenderer->SetEngineState(
        {.lightingMode = LightingModes(properties.lightingMode),
         .renderMode = RenderModes(properties.renderMode),
         .cullingMode = CullingModes(properties.cullingMode),
         .uniformMaterial = {.ambientColor = properties.modelAmbientColor,
                             .ambientIntensity = properties.ambientIntensity,
                             .diffuseColor = properties.modelDiffuseColor,
                             .diffuseIntensity = properties.diffuseIntensity,
                             .specularColor = properties.modelSpecularColor,
                             .specularIntensity = properties.specularIntensity,
                             .shineCoefficient = properties.modelShineCoefficient},
         .polygonOrientation = PolygonOrientation(properties.orientation),
         .backgroundColor = properties.backgroundColor});
}

void PropertyManager::LoadInputFile(const char *fileName)
{
    managedScene->SwapModel(0, Model3D(fileName));
    properties.modelDiffuseColor = managedScene->models[0].materials->diffuseColor;
    properties.modelAmbientColor = managedScene->models[0].materials->ambientColor;
    properties.modelSpecularColor = managedScene->models[0].materials->specularColor;
    properties.modelShineCoefficient = managedScene->models[0].materials->shineCoefficient;
    managedRenderer->SetupBuffers(*managedScene);
    modelLoaded = true;
}

void PropertyManager::ApplyProperties()
{
    RenderWindow();
    ApplyPropertiesToScene();
    ApplyPropertiesToRenderingEngine();
}

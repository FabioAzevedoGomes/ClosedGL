#include "ProgramManager.hpp"

ProgramManager::ProgramManager()
{
    glfwInit();
    window = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "ClosedGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Remove swap interval in GLFW to avoid framerate cap
    gl3wInit();

    InitializeShaders();
    propertyManager = new PropertyManager(window);

    lastTime = glfwGetTime();
    currentTime = glfwGetTime();
}

ProgramManager::~ProgramManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ProgramManager::UpdateFramerate()
{
    frames++;
    currentTime = glfwGetTime();
    if (currentTime - lastTime > 1.0f)
    {
        std::stringstream title;
        title << "ClosedGL | FPS: " << frames;
        glfwSetWindowTitle(window, title.str().c_str());
        lastTime = currentTime;
        frames = 0.0f;
    }
}

void ProgramManager::Run()
{
    Scene *mainScene = new Scene();
    bool test = true;
    while (!glfwWindowShouldClose(window))
    {
        renderingManager.RenderScene(*mainScene);
        propertyManager->AdvanceFrame();
        ApplyPropertiesToScene(propertyManager->properties, mainScene);

        glfwSwapBuffers(window);
        glfwPollEvents();

        UpdateFramerate();
    }
}

void ProgramManager::ApplyRenderingProperties(Properties properties)
{
    renderingManager.SelectEngine(Engines(properties.engine));
    renderingManager.SelectLightingAlgorithm(LightingModes(properties.lightingMode));
    renderingManager.SelectRenderMode(RenderModes(properties.renderMode));
    renderingManager.SelectPolygonOrientation(PolygonOrientation(properties.orientation));
    renderingManager.SelectCullingMode(CullingModes(properties.cullingMode));
    renderingManager.SelectBackgroundColor(properties.backgroundColor);
    renderingManager.SelectRenderUniformColor(properties.modelDiffuseColor,
                                              properties.modelAmbientColor,
                                              properties.modelSpecularColor,
                                              properties.modelShineCoefficient);
}

void ProgramManager::ApplyCameraProperties(Properties &properties, Scene *scene)
{
    if (properties.shouldMove)
    {
        scene->camera.MoveTo(properties.movementDirection, properties.speed);

        if (properties.keepLookingAtModel)
            scene->camera.LookAt();

        properties.shouldMove = false;
    }

    if (properties.resetCamera)
    {
        scene->ResetCamera();

        properties.rotationPitch = 0.0f;
        properties.rotationRoll = 0.0f;
        properties.rotationYaw = 0.0f;

        properties.resetCamera = false;
    }

    if (!properties.keepLookingAtModel)
        scene->camera.Rotate(properties.rotationPitch, properties.rotationRoll, properties.rotationYaw);
    else
        scene->camera.LookAt();

    scene->camera.nearPlane = properties.nearPlane;
    scene->camera.farPlane = properties.farPlane;
    scene->camera.fieldOfView = properties.fieldOfView;
}

void ProgramManager::ApplyPropertiesToScene(Properties &properties, Scene *scene)
{
    if (properties.reloadFile)
    {
        scene->SwapModel(0, Model3D(properties.modelFilePath));
        properties.modelDiffuseColor = scene->models[0].materials->diffuseColor;
        properties.modelAmbientColor = scene->models[0].materials->ambientColor;
        properties.modelSpecularColor = scene->models[0].materials->specularColor;
        properties.modelShineCoefficient = scene->models[0].materials->shineCoefficient;
        renderingManager.SetupBuffers(*scene);
        properties.reloadFile = false;
    }

    ApplyRenderingProperties(properties);
    ApplyCameraProperties(properties, scene);
}

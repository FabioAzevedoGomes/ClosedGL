#include "ProgramManager.hpp"

ProgramManager::ProgramManager(const char *inputFile)
{
    window = new Window("ClosedGL");
    InitializeShaders();
    renderingManager = new RenderingManager();
    mainScene = new Scene();
    propertyManager = new PropertyManager(window->window, mainScene, renderingManager);

    lastTime = glfwGetTime();
    currentTime = glfwGetTime();

    if (inputFile != nullptr)
        propertyManager->LoadInputFile(inputFile);
}

ProgramManager::~ProgramManager()
{
    delete propertyManager;
    delete renderingManager;
    delete mainScene;
    delete window;
}

void ProgramManager::UpdateFramerate()
{
    frames++;
    currentTime = glfwGetTime();
    if (currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        framerate = frames;
        frames = 0.0f;
    }
}

void ProgramManager::Run()
{
    while (!window->ShouldClose())
    {
        UpdateFramerate();
        window->SetTitle(renderingManager->GetActiveEngineName(), framerate);

        renderingManager->RenderSceneToWindow(*mainScene, window);
        propertyManager->ApplyProperties();

        window->Update();
    }
}

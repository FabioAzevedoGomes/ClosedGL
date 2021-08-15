#include "Scene.hpp"

Scene::Scene(GLFWwindow *window)
{
    this->window = window;
    this->models.push_back(Model3D("../model/cow_up.in"));
    this->scenePropertyManager = new PropertyManager(window);

    camera.FrameObject(this->models[0]);

    SetupVAOS(this->models);
    SetupVBOS(this->models);
    SetupShaders();
}

void Scene::ApplyProperties()
{
    SetRenderMode(properties.renderMode);
    // etc.

    properties.changed = false;
}

void Scene::SwapModel(int position, Model3D newModel)
{
    if (position < this->models.size())
        this->models[position] = newModel;
}

void Scene::AdvanceFrame()
{
    ApplyProperties();

    // TODO Any application domain logic goes here

    RenderScene(this->models, camera);
    scenePropertyManager->AdvanceFrame();
}

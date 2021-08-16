#include "Scene.hpp"

Scene::Scene(GLFWwindow *window)
{
    this->window = window;
    this->models.push_back(Model3D("../model/cow_up.in"));

    this->scenePropertyManager = new PropertyManager(window);
    this->scenePropertyManager->properties.modelColor = this->models[0].triangles[0].vertices->color;

    camera.FrameObject(this->models[0]);

    SetupVAOS(this->models);
    SetupVBOS(this->models);
    SetupShaders();
}

void Scene::SwapModel(int position, Model3D newModel)
{
    if (position < this->models.size())
    {
        this->models[position] = newModel;

        camera.ResetPosition();
        camera.FrameObject(this->models[0]);

        this->scenePropertyManager->properties.modelColor = this->models[0].triangles[0].vertices->color;
        this->scenePropertyManager->properties.reloadFile = false;
        this->scenePropertyManager->properties.rotationPitch = 0.0f;
        this->scenePropertyManager->properties.rotationRoll = 0.0f;
        this->scenePropertyManager->properties.rotationYaw = 0.0f;

        SetupVAOS(this->models);
        SetupVBOS(this->models);
        SetupShaders();
    }
}

void Scene::AdvanceFrame()
{
    if (scenePropertyManager->properties.reloadFile)
        SwapModel(0, Model3D(scenePropertyManager->properties.modelFilePath));

    RenderScene(models, camera, scenePropertyManager->properties);
    scenePropertyManager->AdvanceFrame();
}

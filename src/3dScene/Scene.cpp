#include "Scene.hpp"

Scene::Scene(GLFWwindow *window)
{
    this->window = window;
    this->scenePropertyManager = new PropertyManager(window);
    SetupShaders();
}

void Scene::MoveCamera()
{
    camera.MoveTo(scenePropertyManager->properties.movementDirection, scenePropertyManager->properties.speed);

    if (scenePropertyManager->properties.keepLookingAtModel)
        camera.LookAt();

    scenePropertyManager->properties.shouldMove = false;
}

void Scene::ResetCamera()
{
    camera.Reset();
    camera.FrameObject(this->models[0]);

    this->scenePropertyManager->properties.rotationPitch = 0.0f;
    this->scenePropertyManager->properties.rotationRoll = 0.0f;
    this->scenePropertyManager->properties.rotationYaw = 0.0f;

    this->scenePropertyManager->properties.resetCamera = false;
}

void Scene::SwapModel(int position, Model3D newModel)
{
    if (this->models.size() <= 0)
        this->models.push_back(newModel);
    else if (position < this->models.size())
        this->models[position] = newModel;

    this->scenePropertyManager->properties.reloadFile = false;
    this->scenePropertyManager->properties.modelColor = this->models[position].triangles[0].vertices->color;
    ResetCamera();

    SetupVAOS(this->models);
    SetupVBOS(this->models);
}

void Scene::AdvanceFrame()
{
    if (scenePropertyManager->properties.reloadFile)
        SwapModel(0, Model3D(scenePropertyManager->properties.modelFilePath));

    if (scenePropertyManager->properties.resetCamera)
        ResetCamera();

    if (scenePropertyManager->properties.shouldMove)
        MoveCamera();

    RenderScene(models, camera, scenePropertyManager->properties);
    scenePropertyManager->AdvanceFrame();
}

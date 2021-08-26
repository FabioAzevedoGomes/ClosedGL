#include "Scene.hpp"

Scene::Scene()
{
}

void Scene::MoveCamera(MovementOptions direction, float speed, bool keepLookingAtObject)
{
    camera.MoveTo(direction, speed);

    if (keepLookingAtObject)
        camera.LookAt();
}

void Scene::ResetCamera()
{
    camera.Reset();
    camera.FrameObject(this->models[0]);
}

void Scene::SwapModel(int position, Model3D newModel)
{
    if (position < this->models.size())
        this->models[position] = newModel;
    else
        this->models.push_back(newModel);

    ResetCamera();
}

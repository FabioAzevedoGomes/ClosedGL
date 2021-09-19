#pragma once

#include "Properties.hpp"
#include "Camera.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

class Scene
{
public:
    Camera camera;
    std::vector<Model3D> models;

    Scene();
    void MoveCamera(MovementOptions direction, float speed, bool keepLookingAtObject);
    void ResetCamera();
    void SwapModel(int position, Model3D newModel);
    void AdvanceFrame();
};

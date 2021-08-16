#ifndef SCENE_H
#define SCENE_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "Properties.hpp"
#include "RenderUtils.hpp"
#include "PropertyManager.hpp"
#include "Camera.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

class Scene
{
public:
    GLFWwindow *window;

    Camera camera;
    std::vector<Model3D> models;
    PropertyManager *scenePropertyManager;

    Scene(GLFWwindow *window);
    void ResetCamera();
    void SwapModel(int position, Model3D newModel);
    void AdvanceFrame();
};

#endif

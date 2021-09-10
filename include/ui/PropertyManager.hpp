#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileBrowser.h"

#include "Properties.hpp"
#include "Scene.hpp"
#include "RenderingManager.hpp"

#include <iostream>

class PropertyManager
{
private:
    imgui_addons::ImGuiFileBrowser fileDialog;
    bool modelLoaded;

    Properties properties;
    Scene *managedScene;
    RenderingManager *managedRenderer;

    void RenderWelcomeWindow();
    void RenderPropertyWindow();
    void RenderWindow();

    void ApplyPropertiesToScene();
    void ApplyPropertiesToRenderingEngine();

public:
    PropertyManager(GLFWwindow *, Scene *, RenderingManager *);
    ~PropertyManager();

    void LoadInputFile(const char *fileName);
    void ApplyProperties();
};

#endif

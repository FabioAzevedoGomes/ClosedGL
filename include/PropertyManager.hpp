#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "Properties.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileBrowser.h"

#include <iostream>

#define NOP ((void)0)

class PropertyManager
{
public:
    GLFWwindow *window;
    Properties properties;

    imgui_addons::ImGuiFileBrowser fileDialog;

    PropertyManager(GLFWwindow *window);

    void CheckProperties();
    void RenderWindow();
    void AdvanceFrame();
};

#endif

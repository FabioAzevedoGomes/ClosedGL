#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "Properties.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

class PropertyManager
{
public:
    GLFWwindow *window;
    bool showWindow;
    Properties properties;

    PropertyManager(GLFWwindow *window);
    void RenderWindow();
    void AdvanceFrame();
};

#endif

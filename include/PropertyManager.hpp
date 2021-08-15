#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

class PropertyManager
{
public:
    GLFWwindow *window;

    PropertyManager(GLFWwindow *window);
    void AdvanceFrame();
};

#endif

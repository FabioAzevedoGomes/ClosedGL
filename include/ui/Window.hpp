#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <sstream>

#define MAIN_WINDOW_WIDTH 800.0f
#define MAIN_WINDOW_HEIGHT 700.0f

class Window
{
private:
    float width, height;

public:
    GLFWwindow *window;

    Window(const char *title, float width = MAIN_WINDOW_WIDTH, float height = MAIN_WINDOW_HEIGHT);
    ~Window();

    void Resize(float width, float height);
    void SetTitle(std::string renderingEngine, float framerate);
    void Update();

    bool ShouldClose();
    float GetWidth();
    float GetHeight();
};

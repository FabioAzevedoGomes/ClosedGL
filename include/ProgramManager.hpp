#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderingManager.hpp"
#include "PropertyManager.hpp"
#include "ShaderUtils.hpp"
#include "Window.hpp"

#include <sstream>

class ProgramManager
{
private:
    RenderingManager *renderingManager;
    PropertyManager *propertyManager;
    Scene *mainScene;
    Window *window;

    float framerate;
    double lastTime, currentTime;
    int frames;

    void UpdateFramerate();

public:
    ProgramManager(const char *inputModel);
    ~ProgramManager();
    void Run();
};

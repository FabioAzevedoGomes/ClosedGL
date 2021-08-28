#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderingManager.hpp"
#include "PropertyManager.hpp"
#include "ShaderUtils.hpp"

#include <sstream>

class ProgramManager
{
private:
    RenderingManager renderingManager;
    PropertyManager *propertyManager;
    GLFWwindow *window;
    Scene *mainScene;

    double lastTime, currentTime;
    int frames;

    void ApplyPropertiesToScene(Properties &properties, Scene *scene);
    void ApplyCameraProperties(Properties &properties, Scene *scene);
    void ApplyRenderingProperties(Properties properties);
    void UpdateFramerate();

public:
    ProgramManager();
    ~ProgramManager();
    void Run();
};

#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.hpp"
#include "ShaderUtils.hpp"
#include "Window.hpp"

#include <stdio.h>
#include <vector>

#define BUFFER_OFFSET(a) ((void *)(a))

enum Attrib_IDs
{
    openGLvertexPosition = 0,
    openGLvertexColor = 1,
    openGLvertexNormals = 2,
    close2GLvertexPosition = 3,
    close2GLtextureCoordinates = 4
};

class Renderer
{
private:
    virtual void DrawObject(Model3D){};

public:
    glm::vec3 backgroundColor;
    Engines engineId;
    std::string engineName;

    virtual void RenderSceneToWindow(Scene, Window *){};
    virtual void SetupVBOS(std::vector<Model3D>){};
    virtual void SetupVAOS(){};

    virtual void SetCullingMode(CullingModes){};
    virtual void SetPolygonOrientation(PolygonOrientation){};
    virtual void SetRenderMode(RenderModes){};
    virtual void SetBackgroundColor(glm::vec3){};
};

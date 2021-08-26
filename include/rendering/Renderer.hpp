#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.hpp"
#include "ShaderUtils.hpp"

#include <stdio.h>
#include <vector>

#define BUFFER_OFFSET(a) ((void *)(a))

enum Attrib_IDs
{
    openGLvertexPosition = 0,
    openGLvertexColor = 1,
    openGLvertexNormals = 2,
    close2GLvertexPosition = 3
};

class Renderer
{
public:
    glm::vec3 backgroundColor;

    virtual void RenderScene(Scene){};
    virtual void BindObjectBuffers(Model3D){};
    virtual void SetupVBOS(std::vector<Model3D>){};
    virtual void SetupVAOS(std::vector<Model3D>){};
};
#pragma once

#include "Renderer.hpp"

enum OpenGL_Buffer_IDs
{
    VertexPositionBuffer,
    VertexNormalBuffer,
    VertexColorBuffer,
    NumBuffers
};
enum OpenGL_VAO_IDs
{
    ModelObject,
    NumVAOs
};

class OpenGLRenderer : public Renderer
{
private:
    void DrawObject(Model3D object);

    GLuint Buffers[NumBuffers];
    GLuint VAOs[NumVAOs];

public:
    OpenGLRenderer();
    virtual void RenderScene(Scene);
    virtual void BindObjectBuffers(Model3D);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS(std::vector<Model3D>);
};

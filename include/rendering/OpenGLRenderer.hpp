#pragma once

#include "Renderer.hpp"

enum OpenGL_Buffer_IDs
{
    OpenGL_VertexPositionBuffer,
    OpenGL_VertexNormalBuffer,
    OpenGL_VertexColorBuffer,
    OpenGL_NumBuffers
};
enum OpenGL_VAO_IDs
{
    ModelObject_OpenGL,
    NumVAOs_OpenGL
};

class OpenGLRenderer : public Renderer
{
private:
    void DrawObject(Model3D object);

    GLuint Buffers[OpenGL_NumBuffers];
    GLuint VAOs[NumVAOs_OpenGL];

public:
    OpenGLRenderer();
    virtual void RenderScene(Scene);
    virtual void BindObjectBuffers(Model3D);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS();

    virtual void SetCullingMode(CullingModes);
    virtual void SetPolygonOrientation(PolygonOrientation);
};

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
    GLuint Buffers[OpenGL_NumBuffers];
    GLuint VAOs[NumVAOs_OpenGL];

    void BindObjectBuffers(Model3D);
    void DrawObject(Model3D object);

public:
    OpenGLRenderer();
    virtual void RenderSceneToWindow(Scene, Window *);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS();

    virtual void SetCullingMode(CullingModes);
    virtual void SetPolygonOrientation(PolygonOrientation);
    virtual void SetRenderMode(RenderModes);
};

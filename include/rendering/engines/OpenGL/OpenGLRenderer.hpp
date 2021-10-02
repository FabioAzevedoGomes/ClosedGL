#pragma once

#include "Renderer.hpp"

enum OpenGL_Buffer_IDs
{
    OpenGL_VertexPositionBuffer,
    OpenGL_VertexNormalBuffer,
    OpenGL_VertexColorBuffer,
    OpenGL_VertexTexCoordBuffer,
    OpenGL_NumBuffers
};

enum OpenGL_VAO_IDs
{
    ModelObject_OpenGL,
    NumVAOs_OpenGL
};

enum OpenGL_Texture_IDs
{
    ModelTexture_OpenGL,
    NumTextures_OpenGL
};

class OpenGLRenderer : public Renderer
{
private:
    GLuint Buffers[OpenGL_NumBuffers];
    GLuint VAOs[NumVAOs_OpenGL];
    GLuint Textures[NumTextures_OpenGL];

    void BindObjectBuffers(Model3D);
    void DrawObject(Model3D object);

    void SetCullingMode(CullingModes);
    void SetPolygonOrientation(PolygonOrientation);
    void SetRenderMode(RenderModes);
    void SetLightingAlgorithm(LightingModes);
    void SetRenderUniformMaterial(Material);
    void SetResamplingMode(ResamplingModes);

public:
    OpenGLRenderer();
    virtual void RenderSceneToWindow(Scene, Window *);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS();

    virtual void SetState(State);
};

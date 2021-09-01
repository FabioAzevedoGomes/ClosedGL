#pragma once

#include "Renderer.hpp"

enum Close2GL_Buffer_IDs
{
    Close2GL_VertexPositionBuffer,
    Close2GL_NumBuffers
};
enum Close2GL_VAO_IDs
{
    ModelObject_Close2GL,
    NumVAOs_Close2GL
};

class Close2GLRenderer : public Renderer
{
private:
    glm::mat4 model, view, projection;

    CullingModes cullingMode;
    PolygonOrientation polygonOrientation;

    GLuint Buffers[Close2GL_NumBuffers];
    GLuint VAOs[NumVAOs_Close2GL];

    virtual void DrawObject(Model3D);
    bool ShouldCull(std::vector<glm::vec4>);
    int PopulateVertexBuffer(Model3D, float *);

public:
    Close2GLRenderer();
    virtual void RenderScene(Scene);
    virtual void BindObjectBuffers(Model3D);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS();

    virtual void SetCullingMode(CullingModes);
    virtual void SetPolygonOrientation(PolygonOrientation);
};

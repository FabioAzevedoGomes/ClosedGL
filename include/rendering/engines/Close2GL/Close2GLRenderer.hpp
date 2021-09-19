#pragma once

#include <map>

#include "Renderer.hpp"
#include "Close2GLRenderingUtils.hpp"
#include "WireframeRasterizationStrategy.hpp"
#include "FilledRasterizationStrategy.hpp"
#include "PointsRasterizationStrategy.hpp"

enum Close2GL_Buffer_IDs
{
    Close2GL_VertexPositionBuffer,
    Close2GL_VertexTextureCoordinateBuffer,
    Close2GL_NumBuffers
};

enum Close2GL_VAO_IDs
{
    ModelObject_Close2GL,
    NumVAOs_Close2GL
};

enum Close2GL_Texture_IDs
{
    ModelTexture_Close2GL,
    Close2GL_NumTextures
};

class Close2GLRenderer : public Renderer
{
private:
    glm::mat4 model, view, projection, viewport;
    std::map<RenderModes, RasterizationStrategy *> rasterizationStrategies;
    Buffer *buffers;

    GLuint Buffers[Close2GL_NumBuffers];
    GLuint VAOs[NumVAOs_Close2GL];
    GLuint Textures[Close2GL_NumTextures];

    virtual void DrawObject(Model3D);

    bool ShouldCull(Triangle);
    void CalculateRenderingMatrices(Scene, Window *);
    void ClearAndResizeBuffersForWindow(Window *);

public:
    Close2GLRenderer();
    ~Close2GLRenderer();

    virtual void RenderSceneToWindow(Scene, Window *);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS();

    virtual void SetState(State);
};

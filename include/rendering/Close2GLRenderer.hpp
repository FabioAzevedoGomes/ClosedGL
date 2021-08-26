#pragma once

#include "Renderer.hpp"

class Close2GLRenderer : public Renderer
{

public:
    Close2GLRenderer();
    virtual void RenderScene(Scene);
    virtual void BindObjectBuffers(Model3D);
    virtual void SetupVBOS(std::vector<Model3D>);
    virtual void SetupVAOS(std::vector<Model3D>);
};

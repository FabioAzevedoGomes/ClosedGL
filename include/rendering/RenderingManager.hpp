#pragma once

#include "OpenGLRenderer.hpp"
#include "Close2GLRenderer.hpp"

class RenderingManager
{
private:
    OpenGLRenderer openGLRenderer;
    Close2GLRenderer close2GLRenderer;
    Renderer *selectedRenderingEngine;

public:
    RenderingManager();

    void SetupBuffers(Scene);
    void RenderSceneToWindow(Scene, Window *);

    std::string GetActiveEngineName();

    void SelectEngine(Engines, Scene);
    void SetEngineState(State);
};

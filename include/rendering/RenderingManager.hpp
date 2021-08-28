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
    void RenderScene(Scene);

    void SelectEngine(Engines, Scene);
    void SelectLightingAlgorithm(LightingModes);
    void SelectRenderMode(RenderModes);
    void SelectCullingMode(CullingModes);
    void SelectRenderUniformColor(glm::vec3, glm::vec3, glm::vec3, float);
    void SelectBackgroundColor(glm::vec3);
    void SelectPolygonOrientation(PolygonOrientation);
};

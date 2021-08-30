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

    std::string GetActiveEngineName();

    void SelectEngine(Engines, Scene);
    void SelectLightingAlgorithm(LightingModes);
    void SelectRenderMode(RenderModes);
    void SelectCullingMode(CullingModes);
    void SelectRenderUniformColor(glm::vec3, float, glm::vec3, float, glm::vec3, float, float);
    void SelectBackgroundColor(glm::vec3);
    void SelectPolygonOrientation(PolygonOrientation);
};

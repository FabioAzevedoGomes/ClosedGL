#pragma once

#include <glm/vec3.hpp>

#include "Properties.hpp"

typedef struct t_state
{
    LightingModes lightingMode;
    RenderModes renderMode;
    CullingModes cullingMode;
    Material uniformMaterial;
    PolygonOrientation polygonOrientation;
    glm::vec3 backgroundColor;
} State;

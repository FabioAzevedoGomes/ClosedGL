#pragma once

#include "Camera.hpp"
#include "Triangle.hpp"
#include "Material.hpp"

#define LIGHT_SOURCE_POSITION glm::vec4(2.0f,2.0f,2.0f,1.0f)
#define SHADOW_MIN_INTENSITY 0.2f

class VertexLightingStrategy
{
protected:
    Material uniformMaterial;

public:
    void SetUniformMaterial(Material);

    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &){};
};

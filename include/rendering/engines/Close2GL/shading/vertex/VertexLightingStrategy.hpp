#pragma once

#include "Camera.hpp"
#include "Triangle.hpp"
#include "Material.hpp"
#include "ShadingConstants.hpp"

class VertexLightingStrategy
{
protected:
    Material uniformMaterial;

public:
    void SetUniformMaterial(Material);

    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &){};
};

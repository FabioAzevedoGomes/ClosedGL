#pragma once

#include "Camera.hpp"
#include "Triangle.hpp"
#include "Material.hpp"

class LightingStrategy
{
protected:
    Material uniformMaterial;

public:
    void SetUniformMaterial(Material);

    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &){};
};

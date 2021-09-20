#pragma once

#include "VertexLightingStrategy.hpp"

class PhongLightingStrategyVertex : public VertexLightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &triangle, Camera &camera);
};

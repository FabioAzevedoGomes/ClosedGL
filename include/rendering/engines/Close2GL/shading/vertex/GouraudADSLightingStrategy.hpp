#pragma once

#include "VertexLightingStrategy.hpp"

class GouraudADSLightingStrategy : public VertexLightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &triangle, Camera &camera);
};

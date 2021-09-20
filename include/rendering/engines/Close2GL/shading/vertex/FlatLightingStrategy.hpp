#pragma once

#include "VertexLightingStrategy.hpp"

class FlatLightingStrategy : public VertexLightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &);
};

#pragma once

#include "VertexLightingStrategy.hpp"

class GouraudADLightingStrategy : public VertexLightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &);
};

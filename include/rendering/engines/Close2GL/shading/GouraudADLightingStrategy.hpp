#pragma once

#include "LightingStrategy.hpp"

class GouraudADLightingStrategy : public LightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &);
};

#pragma once

#include "LightingStrategy.hpp"

class FlatLightingStrategy : public LightingStrategy
{
public:
    virtual void ShadeTriangleRelativeToCamera(Triangle &, Camera &);
};

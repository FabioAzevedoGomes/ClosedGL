#pragma once

#include "RasterizationStrategy.hpp"

class PointsRasterizationStrategy : public RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer);
};
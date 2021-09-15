#pragma once

#include "RasterizationStrategy.hpp"

class WireframeRasterizationStrategy : public RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(Triangle triangle, float ***colorBuffer, float **depthBuffer);
};
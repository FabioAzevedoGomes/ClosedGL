#pragma once

#include "RasterizationStrategy.hpp"

class FilledRasterizationStrategy : public RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(Triangle triangle, float ***colorBuffer, float **depthBuffer);
};
#pragma once

#include "RasterizationStrategy.hpp"

class FilledRasterizationStrategy : public RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(std::vector<glm::vec4> triangle, float ***colorBuffer, float **depthBuffer);
};
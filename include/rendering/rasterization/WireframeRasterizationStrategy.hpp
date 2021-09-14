#pragma once

#include "RasterizationStrategy.hpp"

class WireframeRasterizationStrategy : public RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(std::vector<glm::vec4> triangle, float ***colorBuffer, float **depthBuffer);
};
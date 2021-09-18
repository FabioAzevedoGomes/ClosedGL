#pragma once

#include "RasterizationStrategy.hpp"

class WireframeRasterizationStrategy : public RasterizationStrategy
{
private:
    void DrawAlongScanlineForEdge(int index);

public:
    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer);
};

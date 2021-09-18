#pragma once

#include "RasterizationStrategy.hpp"

class FilledRasterizationStrategy : public RasterizationStrategy
{
private:
    void DrawAlongScanline();
    void DrawHorizontalEdge(int index);
public:
    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer);
};
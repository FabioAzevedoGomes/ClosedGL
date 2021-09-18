#pragma once

#include "RasterizationStrategy.hpp"

class FilledRasterizationStrategy : public RasterizationStrategy
{
private:
    void DrawAlongScanline();

public:
    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer);
};
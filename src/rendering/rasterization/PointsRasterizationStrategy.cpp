#include "PointsRasterizationStrategy.hpp"

void PointsRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, Buffer *buffer)
{
    this->buffers = buffer;
    for (int i = 0; i < 3; i++)
        drawInterpolatedVertexToBuffer(triangle.vertices[i]);
}

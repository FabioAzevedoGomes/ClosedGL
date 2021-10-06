#include "FilledRasterizationStrategy.hpp"

void FilledRasterizationStrategy::DrawAlongScanline()
{
    DrawAlongScanlineForEdge(0);
    DrawAlongScanlineForEdge(1);

    Vertex left, right, point;
    if (activeEdges[0].currentX < activeEdges[1].currentX)
    {
        interpolateLinearlyOverEdge(activeEdges[0], left, activeEdges[0].currentX, currentY, activeEdges[0].currentZ);
        interpolateLinearlyOverEdge(activeEdges[1], right, activeEdges[1].currentX, currentY, activeEdges[1].currentZ);
    }
    else
    {
        interpolateLinearlyOverEdge(activeEdges[0], right, activeEdges[0].currentX, currentY, activeEdges[0].currentZ);
        interpolateLinearlyOverEdge(activeEdges[1], left, activeEdges[1].currentX, currentY, activeEdges[1].currentZ);
    }

    if (left.position.x != right.position.x)
    {
        drawInterpolatedVertexToBuffer(left);
        drawInterpolatedVertexToBuffer(right);

        Edge scanlineEdge(left, right, -1);

        for (float currentX = left.position.x, currentZ = left.position.z;
             currentX <= std::floor(right.position.x);
             currentX++, currentZ += scanlineEdge.dz / scanlineEdge.dx)
        {
            interpolateLinearlyOverEdge(scanlineEdge, point, currentX, currentY, currentZ);
            drawInterpolatedVertexToBuffer(point);
        }
    }
}

void FilledRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, Buffer *buffer)
{
    this->buffers = buffer;

    SetupForRasterization(triangle);

    for (int i = 0; i < 3; i++)
        drawInterpolatedVertexToBuffer(triangle.vertices[i]);

    while (currentY <= maximumY + 1)
    {
        DrawAlongScanline();

        if (std::floor(activeEdges[0].end.position.y) <= std::floor(currentY + 1))
            SwapActiveEdge(triangle, 0);

        if (std::floor(activeEdges[1].end.position.y) <= std::floor(currentY + 1))
            SwapActiveEdge(triangle, 1);

        activeEdges[0].Increment();
        activeEdges[1].Increment();

        currentY++;
    }
}

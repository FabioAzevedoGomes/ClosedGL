#include "WireframeRasterizationStrategy.hpp"

#define DEBUG_WIREFRAME false

void WireframeRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, Buffer *buffer)
{
    this->buffers = buffer;

    SetupForRasterization(triangle);

    for (int i = 0; i < 3; i++)
        drawInterpolatedVertexToBuffer(triangle.vertices[i]);

    while (currentY <= maximumY + 1)
    {
        for (int activeEdge = 0; activeEdge < 2; activeEdge++)
        {
            DrawAlongScanlineForEdge(activeEdge);

            if (std::floor(activeEdges[activeEdge].end.position.y) <= std::floor(currentY + 1))
                SwapActiveEdge(triangle, activeEdge);

            activeEdges[activeEdge].Increment();
        }

        currentY++;
    }
}

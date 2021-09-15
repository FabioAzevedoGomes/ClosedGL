#include "FilledRasterizationStrategy.hpp"

void sortVerticesFromHigherToLower(Triangle &triangle)
{
    Vertex aux;
    if (triangle.vertices[0].position.y > triangle.vertices[1].position.y)
    {
        aux = triangle.vertices[0];
        triangle.vertices[0] = triangle.vertices[1];
        triangle.vertices[1] = aux;
    }
    if (triangle.vertices[0].position.y > triangle.vertices[2].position.y)
    {
        aux = triangle.vertices[0];
        triangle.vertices[0] = triangle.vertices[2];
        triangle.vertices[2] = aux;
    }
    if (triangle.vertices[1].position.y > triangle.vertices[2].position.y)
    {
        aux = triangle.vertices[1];
        triangle.vertices[1] = triangle.vertices[2];
        triangle.vertices[2] = aux;
    }
}

void drawInterpolatedVertexToBuffer(Vertex vertex, float ***colorBuffer, float **depthBuffer)
{
}

Vertex interpolateProperties(Vertex &start, Vertex &end, float alpha)
{
}

void FilledRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, float ***colorBuffer, float **depthBuffer)
{
    // Pseudo-algorithm
    //
    // 1. Find top vertex
    sortVerticesFromHigherToLower(triangle);
    // 2. For each edge
    float dx[3], dy[3], dz[3], incX[3], incZ[3];
    for (int edge = 0; edge < 3; edge++)
    {
        //   2.2 Calculate dx, dy, dz, incX, incZ (downwards)
        dx[edge] = triangle.vertices[(edge + 1) % 3].position.x - triangle.vertices[edge].position.x;
        dy[edge] = triangle.vertices[(edge + 1) % 3].position.y - triangle.vertices[edge].position.y;
        dz[edge] = triangle.vertices[(edge + 1) % 3].position.z - triangle.vertices[edge].position.z;
        incX[edge] = dx[edge] / dy[edge];
        incZ[edge] = dz[edge] / dy[edge];
    }
    // 3. Define active edges
    Edge edge1 = {.start = triangle.vertices[0], .end = triangle.vertices[1]};
    float edge1currentX = edge1.start.position.x;
    float edge1currentZ = edge1.start.position.z;
    int edge1Num = 0;
    Edge edge2 = {.start = triangle.vertices[0], .end = triangle.vertices[2]};
    float edge2currentX = edge2.start.position.x;
    float edge2currentZ = edge2.start.position.z;
    int edge2Num = 2;
    // 4. while (y > ymin)
    float currentY = triangle.vertices[0].position.y;
    float minimumY = triangle.vertices[2].position.y;
    while (currentY >= minimumY)
    {
        // 4.3 Interpolate values for active fragments in both active edges
        // 4.4 Multiply interpolated values by interpolated wp
        if (edge1currentX == edge2currentX)
        {
            if (std::floor(currentY) == std::floor(minimumY))
                drawInterpolatedVertexToBuffer(edge1.end, colorBuffer, depthBuffer);
            else
                drawInterpolatedVertexToBuffer(edge1.start, colorBuffer, depthBuffer);
        }
        else
        {
            Vertex edge1Point = interpolateProperties(edge1.start, edge1.end, (edge1currentX - edge1.start.position.x) / dx[edge1Num]);
            Vertex edge2Point = interpolateProperties(edge2.start, edge2.end, (edge2currentX - edge2.start.position.x) / dx[edge2Num]);

            drawInterpolatedVertexToBuffer(edge1Point, colorBuffer, depthBuffer);
            drawInterpolatedVertexToBuffer(edge2Point, colorBuffer, depthBuffer);

            // 4.5 For each fragment between both active fragments
            for (int i = std::floor(std::min(edge1Point.position.x, edge2Point.position.x));
                 i < std::floor(std::max(edge1Point.position.x, edge2Point.position.x));
                 i++)
            {
                // 4.5.1 Interpolate values for fragment
                // 4.5.2 Multiply interpolated values by interpolated wp
                Vertex interpolatedFragment = interpolateProperties(edge1Point,
                                                                            edge2Point,
                                                                            (i - std::floor(std::min(edge1Point.position.x, edge2Point.position.x))) 
                                                                            / (std::floor(std::max(edge1Point.position.x, edge2Point.position.x)) 
                                                                            - std::floor(std::min(edge1Point.position.x, edge2Point.position.x))));
                drawInterpolatedVertexToBuffer(interpolatedFragment, colorBuffer, depthBuffer);
            }
        }
        // 4.6 Decrease y
        currentY--;
        // 4.1 xi = xi-1 + inciX (both active edges)
        // 4.2 zi = zi-1 + inciZ (both active edges)
        edge1currentX += incX[edge1Num];
        edge1currentZ += incZ[edge1Num];
        edge2currentX += incX[edge2Num];
        edge2currentZ += incZ[edge2Num];
        // 4.7 Check if only one active edge ended
        //   4.8 If so, swap to third edge
        if (std::floor(edge1.end.position.y) == std::floor(currentY) && std::floor(edge2.end.position.y) != std::floor(currentY))
        {
            edge1 = {.start = triangle.vertices[1], .end = triangle.vertices[2]};
            edge1currentX = edge1.start.position.x;
            edge1currentZ = edge1.start.position.z;
            edge1Num = 1;
        }
        if (std::floor(edge2.end.position.y) == std::floor(currentY) && std::floor(edge1.end.position.y) != std::floor(currentY))
        {
            edge2 = {.start = triangle.vertices[1], .end = triangle.vertices[2]};
            edge2currentX = edge2.start.position.x;
            edge2currentZ = edge2.start.position.z;
            edge2Num = 1;
        }
    }
}

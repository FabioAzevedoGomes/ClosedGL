#include "FilledRasterizationStrategy.hpp"

void FilledRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, Buffer *buffer)
{
    // Pseudo-algorithm
    //
    // 1. Find top vertex
    sortVerticesFromHigherToLower(triangle.vertices);
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
    Vertex edge1Point, edge2Point, middleFragment;
    // 4. while (y > ymin)
    float currentY = triangle.vertices[0].position.y;
    float minimumY = triangle.vertices[2].position.y;
    std::cout << "Triangle: " << std::endl;
    std::cout << "\t" << glm::to_string(triangle.vertices[0].position) << std::endl;
    std::cout << "\t" << glm::to_string(triangle.vertices[1].position) << std::endl;
    std::cout << "\t" << glm::to_string(triangle.vertices[2].position) << std::endl;
    std::cout << currentY << ", " << minimumY << std::endl;
    drawInterpolatedVertexToBuffer(triangle.vertices[0], buffer);
    while (currentY >= minimumY)
    {
        // 4.6 Decrease y
        currentY--;
        // 4.1 xi = xi-1 + inciX (both active edges)
        // 4.2 zi = zi-1 + inciZ (both active edges)
        edge1currentX += incX[edge1Num];
        edge1currentZ += incZ[edge1Num];
        edge2currentX += incX[edge2Num];
        edge2currentZ += incZ[edge2Num];

        // 4.3 Interpolate values for active fragments in both active edges
        // 4.4 Multiply interpolated values by interpolated wp
        if (edge1currentX == edge2currentX)
        {
            if (std::floor(currentY) == std::floor(minimumY))
                drawInterpolatedVertexToBuffer(edge1.end, buffer);
            else
                drawInterpolatedVertexToBuffer(edge1.start, buffer);
        }
        else
        {
            interpolateProperties(edge1.start, edge1.end, (edge1currentX - edge1.start.position.x) / dx[edge1Num], edge1Point);
            interpolateProperties(edge2.start, edge2.end, (edge2currentX - edge2.start.position.x) / dx[edge2Num], edge2Point);

            drawInterpolatedVertexToBuffer(edge1Point, buffer);
            drawInterpolatedVertexToBuffer(edge2Point, buffer);

            // 4.5 For each fragment between both active fragments
            for (int i = std::floor(std::min(edge1Point.position.x, std::min(edge2Point.position.x, (float)buffer->width)));
                 i < std::floor(std::max(edge1Point.position.x, std::max(edge2Point.position.x, 0.0f)));
                 i++)
            {
                // 4.5.1 Interpolate values for fragment
                // 4.5.2 Multiply interpolated values by interpolated wp
                interpolateProperties(edge1Point,
                                      edge2Point,
                                      (i - std::floor(std::min(edge1Point.position.x, edge2Point.position.x))) / (std::floor(std::max(edge1Point.position.x, edge2Point.position.x)) - std::floor(std::min(edge1Point.position.x, edge2Point.position.x))), middleFragment);
                drawInterpolatedVertexToBuffer(middleFragment, buffer);
            }
        }
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

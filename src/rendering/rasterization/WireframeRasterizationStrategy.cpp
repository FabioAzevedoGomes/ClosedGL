#include "WireframeRasterizationStrategy.hpp"

#define DEBUG_WIREFRAME false

void WireframeRasterizationStrategy::DrawTriangleToBuffer(Triangle triangle, Buffer *buffer)
{
    for (int i = 0; i < 3; i++)
        drawInterpolatedVertexToBuffer(triangle.vertices[i], buffer);

    sortVerticesFromHigherToLower(triangle.vertices);

    if (DEBUG_WIREFRAME)
    {
        std::cout << "Sorted triangle: " << std::endl;
        triangle.print();
    }

    float dx[3], dy[3], dz[3], incX[3], incZ[3];

    dx[0] = std::floor(triangle.vertices[1].position.x) - std::floor(triangle.vertices[0].position.x);
    dy[0] = std::floor(triangle.vertices[1].position.y) - std::floor(triangle.vertices[0].position.y);
    dz[0] = std::floor(triangle.vertices[1].position.z) - std::floor(triangle.vertices[0].position.z);

    incX[0] = dx[0] / dy[0];
    incZ[0] = dz[0] / dy[0];

    dx[1] = std::floor(triangle.vertices[2].position.x) - std::floor(triangle.vertices[1].position.x);
    dy[1] = std::floor(triangle.vertices[2].position.y) - std::floor(triangle.vertices[1].position.y);
    dz[1] = std::floor(triangle.vertices[2].position.z) - std::floor(triangle.vertices[1].position.z);

    incX[1] = dx[1] / dy[1];
    incZ[1] = dz[1] / dy[1];

    dx[2] = std::floor(triangle.vertices[2].position.x) - std::floor(triangle.vertices[0].position.x);
    dy[2] = std::floor(triangle.vertices[2].position.y) - std::floor(triangle.vertices[0].position.y);
    dz[2] = std::floor(triangle.vertices[2].position.z) - std::floor(triangle.vertices[0].position.z);

    incX[2] = dx[2] / dy[2];
    incZ[2] = dz[2] / dy[2];

    if (DEBUG_WIREFRAME) {
        for (int i = 0; i < 3; i++)
            if (dy[i] == 0.0f)
                std::cout << " DY[" << i << "] ZERO" << std::endl;
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
    float currentY = triangle.vertices[0].position.y;
    float maximumY = triangle.vertices[2].position.y;

    // Horizontal top line
    if (triangle.vertices[0].position.y == triangle.vertices[1].position.y)
    {
        int left, right;
        if (triangle.vertices[0].position.x > triangle.vertices[1].position.x)
        {
            left = 1;
            right = 0;
        }
        else
        {
            left = 0;
            right = 1;
        }
        int start = triangle.vertices[left].position.x;
        int end = triangle.vertices[right].position.x;
        for (int x = start, z = triangle.vertices[left].position.z; x <= end; x++, z += dz[0] / dx[0])
        {
            interpolateLinearlyOverEdge({.start = triangle.vertices[left], .end = triangle.vertices[right]}, edge1Point, x, currentY, z);
            drawInterpolatedVertexToBuffer(edge1Point, buffer);
        }

        edge1 = {.start = triangle.vertices[1], .end = triangle.vertices[2]};
        edge1currentX = edge1.start.position.x;
        edge1currentZ = edge1.start.position.z;
        edge1Num = 1;
    }

    // Diagonal lines
    bool edge1Switched = false, edge2Switched = false;
    while (currentY <= maximumY)
    {
        if (DEBUG_WIREFRAME)
        {
            std::cout << std::endl;
            //std::cout << "edge1CurrentX: " << edge1currentX << std::endl;
            //std::cout << "edge1CurrentZ: " << edge1currentZ << std::endl;
            //std::cout << "edge2CurrentX: " << edge2currentX << std::endl;
            //std::cout << "edge2CurrentZ: " << edge2currentZ << std::endl;
            //std::cout << "currentY: " << currentY << std::endl;
            //std::cout << "maximumY: " << maximumY << std::endl;
            //std::cout << "dx[edge1Num]: " << dx[edge1Num] << std::endl;
            //std::cout << "dz[edge1Num]: " << dz[edge1Num] << std::endl;
            //std::cout << "dx[edge2Num]: " << dx[edge2Num] << std::endl;
            //std::cout << "dz[edge2Num]: " << dz[edge2Num] << std::endl;
            //std::cout << "edge1Num: " << edge1Num << std::endl;
            //std::cout << "edge2Num: " << edge2Num << std::endl;
        }
        // 4.3 Interpolate values for active fragments in both active edges
        // 4.4 Multiply interpolated values by interpolated wp

        float minX, minZ, maxX, maxZ;

        if (DEBUG_WIREFRAME)
        {
            std::cout << "edge1CurrentX: " << edge1currentX << std::endl;
            std::cout << "incX[edge1Num]: " << incX[edge1Num] << std::endl;
            std::cout << "edge1Num: " << edge1Num << std::endl;
            std::cout << "\t Interpolating between " << minX << " and " << maxX << std::endl;
            std::cout << "currentY: " << currentY << std::endl;
        }
        
        if (dy[edge1Num] != 0.0f)
        {
            minX = std::min(edge1currentX + incX[edge1Num], edge1currentX);
            minZ = std::min(edge1currentZ + incZ[edge1Num], edge1currentZ);
            maxX = std::max(edge1currentX + incX[edge1Num], edge1currentX);
            maxZ = std::max(edge1currentZ + incZ[edge1Num], edge1currentZ);
            for (int x = minX, z = minZ; x <= maxX; x++, z += dz[edge1Num] / dx[edge1Num])
            {
                if (DEBUG_WIREFRAME) std::cout << "alpha: " << distanceBetween(edge1.start.position, glm::vec4(x, currentY, z, 1.0f)) / distanceBetween(edge1.start.position, edge1.end.position) << std::endl;

                interpolateLinearlyOverEdge(edge1, edge1Point, (float)x, currentY, (float)z);
                drawInterpolatedVertexToBuffer(edge1Point, buffer);
            }
        }

        if (dy[edge2Num] != 0.0f)
        {
            minX = std::min(edge2currentX + incX[edge2Num], edge2currentX);
            minZ = std::min(edge2currentZ + incZ[edge2Num], edge2currentZ);
            maxX = std::max(edge2currentX + incX[edge2Num], edge2currentX);
            maxZ = std::max(edge2currentZ + incZ[edge2Num], edge2currentZ);
            for (int x = minX, z = minZ; x <= maxX; (float)x++, (float)z++)
            {
                interpolateLinearlyOverEdge(edge2, edge2Point, x, currentY, z);
                drawInterpolatedVertexToBuffer(edge2Point, buffer);
            }
        }

        if (!edge1Switched || !edge2Switched)
        {
            edge1Switched = false;
            edge2Switched = false;
        }

        currentY++;

        // 4.7 Check if only one active edge ended
        //   4.8 If so, swap to third edge
        if (std::floor(edge1.end.position.y) <= std::floor(currentY))
        {
            if (DEBUG_WIREFRAME) std::cout << "Switched edge1 at " << edge1currentX << ", " << currentY << ", " << edge1currentZ << std::endl;

            edge1 = {.start = triangle.vertices[1], .end = triangle.vertices[2]};
            edge1currentX = edge1.start.position.x;
            edge1currentZ = edge1.start.position.z;
            edge1Num = 1;
            edge1Switched = true;
        }
        if (std::floor(edge2.end.position.y) <= std::floor(currentY))
        {
            if (DEBUG_WIREFRAME) std::cout << "Switched edge2 at " << edge2currentX << ", " << currentY << ", " << edge2currentZ << std::endl;

            edge2 = {.start = triangle.vertices[1], .end = triangle.vertices[2]};
            edge2currentX = edge2.start.position.x;
            edge2currentZ = edge2.start.position.z;
            edge2Num = 1;
            edge2Switched = true;
        }

        // 4.1 xi = xi-1 + inciX (both active edges)
        // 4.2 zi = zi-1 + inciZ (both active edges)

        if (!edge1Switched)
        {
            edge1currentX += incX[edge1Num];
            edge1currentZ += incZ[edge1Num];
        }
        if (!edge2Switched)
        {
            edge2currentX += incX[edge2Num];
            edge2currentZ += incZ[edge2Num];
        }
    }

    // Horizontal bottom line
    if (triangle.vertices[1].position.y == triangle.vertices[2].position.y)
    {
        int left, right;
        if (triangle.vertices[1].position.x > triangle.vertices[2].position.x)
        {
            left = 2;
            right = 1;
        }
        else
        {
            left = 1;
            right = 2;
        }
        int start = std::floor(std::max(0.0f, triangle.vertices[left].position.x));
        int end = std::floor(std::min((float)buffer->width, triangle.vertices[right].position.x));
        for (int x = start, z = triangle.vertices[left].position.z; x <= end; x++, z += dz[2] / dx[2])
        {
            interpolateLinearlyOverEdge({.start = triangle.vertices[left], .end = triangle.vertices[right]}, edge1Point, x, currentY, z);
            drawInterpolatedVertexToBuffer(edge1Point, buffer);
        }
    }
}

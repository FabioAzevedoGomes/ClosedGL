#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Triangle.hpp"
#include "Buffer.hpp"

class RasterizationStrategy
{
private:
    void SortVerticesFromHigherToLower(Vertex *vertices);
    void InitializeActiveEdges(Triangle &triangle);

protected:
    float currentY, maximumY;
    Edge activeEdges[2];
    Buffer *buffers;

    void SetupForRasterization(Triangle &triangle);
    void SwapActiveEdge(Triangle &triangle, int edgeNumber);
    void DrawAlongScanlineForEdge(int index);

    void drawInterpolatedVertexToBuffer(Vertex &vertex);
    float distanceBetween(glm::vec4 pos1, glm::vec4 pos2);
    void interpolateLinearlyOverEdge(Edge edge, Vertex &interpolated, float currentX, float currentY, float currentZ);

public:
    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer){};
};

#include "Close2GLRenderingUtils.hpp"

bool isInsideNDCFrustum(glm::vec3 vertex)
{
    return vertex.x > -1 && vertex.x < 1 && vertex.y > -1 && vertex.y < 1 && vertex.z > -1 && vertex.z < 1;
}

std::vector<glm::vec4> projectTriangleToNDC(Triangle triangle, glm::mat4 modelViewProj)
{
    std::vector<glm::vec4> projectedVertices;

    bool insideFrustum = false;
    for (int vertex = 0; vertex < 3; vertex++)
    {
        glm::vec4 vertexPosition = glm::vec4(triangle.vertices[vertex].position, 1.0f);
        glm::vec4 projectedPosition = modelViewProj * vertexPosition;

        // Clip triangle when w <= 0
        if (projectedPosition.w <= 0)
        {
            projectedVertices.clear();
            break;
        }

        // Perspective division
        projectedPosition /= projectedPosition.w;

        // Clip triangle before/after Near/Far planes
        if (projectedPosition.z < -1 || projectedPosition.z > 1)
        {
            projectedVertices.clear();
            break;
        }

        if (isInsideNDCFrustum(projectedPosition))
            insideFrustum = true;

        projectedVertices.push_back(projectedPosition);
    }

    if (!insideFrustum)
        projectedVertices.clear();

    return projectedVertices;
}

std::vector<glm::vec4> projectVerticesToViewport(std::vector<glm::vec4> triangleVertices, glm::mat4 viewport)
{
    std::vector<glm::vec4> projectedVertices;

    for (int vertex = 0; vertex < 3; vertex++)
        projectedVertices.push_back(viewport * triangleVertices[vertex]);

    return projectedVertices;
}

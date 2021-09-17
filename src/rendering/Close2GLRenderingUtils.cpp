#include "Close2GLRenderingUtils.hpp"

bool isInsideNDCFrustum(Triangle triangle)
{
    for (int i = 0; i < 3; i++)
        if (triangle.vertices[i].position.x > -1 && triangle.vertices[i].position.x < 1 && triangle.vertices[i].position.y > -1 && triangle.vertices[i].position.y < 1 && triangle.vertices[i].position.z > -1 && triangle.vertices[i].position.z < 1)
            return true;

    return false;
}

void perspectiveDivideVertex(Vertex &vertex)
{
    float wp = vertex.position.w;
    vertex.position = vertex.position / wp;
    vertex.normal = vertex.normal / wp;
    vertex.color = vertex.color / wp;
    vertex.wp = 1 / wp;
}

void projectTriangleToNDC(Triangle &triangle, glm::mat4 modelViewProj)
{
    for (int vertex = 0; vertex < 3; vertex++)
    {
        triangle.vertices[vertex].position = modelViewProj * triangle.vertices[vertex].position;

        // Clip triangle when w <= 0
        if (triangle.vertices[vertex].position.w <= 0)
        {
            std::cout << "Clipped triangle for w <= 0" << std::endl;
            triangle.print();
            triangle.clipped = true;
            break;
        }

        perspectiveDivideVertex(triangle.vertices[vertex]);

        // Clip triangle before/after Near/Far planes
        if (triangle.vertices[vertex].position.z < -1 || triangle.vertices[vertex].position.z > 1)
        {
            std::cout << "Clipped triangle for being before/after near/far plane" << std::endl;
            triangle.print();
            triangle.clipped = true;
            break;
        }
    }

    if (!triangle.clipped && !isInsideNDCFrustum(triangle))
    {
        std::cout << "Clipped triangle for being outside NDC frustum" << std::endl;
        triangle.print();

        triangle.clipped = true;
    }
}

void projectTriangleToViewport(Triangle &triangle, glm::mat4 viewport)
{
    for (int vertex = 0; vertex < 3; vertex++)
        triangle.vertices[vertex].position = viewport * triangle.vertices[vertex].position;
}

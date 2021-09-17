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
public:
    void sortVerticesFromHigherToLower(Vertex *vertices)
    {
        Vertex aux;
        if (vertices[0].position.y > vertices[1].position.y)
        {
            aux = vertices[0];
            vertices[0] = vertices[1];
            vertices[1] = aux;
        }
        if (vertices[0].position.y > vertices[2].position.y)
        {
            aux = vertices[0];
            vertices[0] = vertices[2];
            vertices[2] = aux;
        }
        if (vertices[1].position.y > vertices[2].position.y)
        {
            aux = vertices[1];
            vertices[1] = vertices[2];
            vertices[2] = aux;
        }
    }

    void drawInterpolatedVertexToBuffer(Vertex &vertex, Buffer *buffer)
    {
        vertex.position.z *= vertex.wp;
        buffer->draw(vertex.position, glm::vec4(1.0f));
    }

    void interpolateProperties(Vertex &start, Vertex &end, float alpha, Vertex &interpolated)
    {
        interpolated.position = start.position + alpha * (end.position - start.position);
        interpolated.normal = start.normal + alpha * (end.normal - start.normal);
        interpolated.color = start.color + alpha * (end.color - start.color);
        interpolated.wp = start.wp + alpha * (end.wp - start.wp);
    }

    float distanceBetween(glm::vec4 pos1, glm::vec4 pos2)
    {
        return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2) + std::pow(pos1.z - pos2.z, 2));
    }

    void interpolateLinearlyOverEdge(Edge edge, Vertex &interpolated, float currentX, float currentY, float currentZ)
    {
        float alpha = distanceBetween(edge.start.position, glm::vec4(currentX, currentY, currentZ, 1.0f)) /
                      distanceBetween(edge.start.position, edge.end.position);

        interpolated.position = edge.start.position + alpha * (edge.end.position - edge.start.position);
        interpolated.normal = edge.start.normal + alpha * (edge.end.normal - edge.start.normal);
        interpolated.color = edge.start.color + alpha * (edge.end.color - edge.start.color);
        interpolated.wp = edge.start.wp + alpha * (edge.end.wp - edge.start.wp);
    }

    virtual void DrawTriangleToBuffer(Triangle triangle, Buffer *buffer){};
};

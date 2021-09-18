#include "RasterizationStrategy.hpp"

void RasterizationStrategy::SortVerticesFromHigherToLower(Vertex *vertices)
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

void RasterizationStrategy::InitializeActiveEdges(Triangle &triangle)
{
    for (int activeEdge = 0; activeEdge < 2; activeEdge++)
    {
        activeEdges[activeEdge] = Edge(triangle.vertices[0], triangle.vertices[activeEdge + 1], activeEdge * 2);
        activeEdges[activeEdge].justSwapped = false;
    }
}

void RasterizationStrategy::SetupForRasterization(Triangle &triangle)
{
    SortVerticesFromHigherToLower(triangle.vertices);
    InitializeActiveEdges(triangle);

    currentY = triangle.vertices[0].position.y;
    maximumY = triangle.vertices[2].position.y;
}

void RasterizationStrategy::SwapActiveEdge(Triangle &triangle, int edgeNumber)
{
    activeEdges[edgeNumber] = Edge(triangle.vertices[1], triangle.vertices[2], 1);
}

void RasterizationStrategy::drawInterpolatedVertexToBuffer(Vertex &vertex)
{
    vertex.position.z *= vertex.wp;
    buffers->draw(vertex.position, glm::vec4(1.0f));
}

float RasterizationStrategy::distanceBetween(glm::vec4 pos1, glm::vec4 pos2)
{
    return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2) + std::pow(pos1.z - pos2.z, 2));
}

void RasterizationStrategy::interpolateLinearlyOverEdge(Edge edge, Vertex &interpolated, float currentX, float currentY, float currentZ)
{
    float edgeDistance = distanceBetween(edge.start.position, edge.end.position);
    float pointDistance = distanceBetween(edge.start.position, glm::vec4(currentX, currentY, currentZ, 1.0f));
    float alpha = pointDistance / edgeDistance;

    interpolated.position = edge.start.position + alpha * (edge.end.position - edge.start.position);
    interpolated.normal = edge.start.normal + alpha * (edge.end.normal - edge.start.normal);
    interpolated.color = edge.start.color + alpha * (edge.end.color - edge.start.color);
    interpolated.wp = edge.start.wp + alpha * (edge.end.wp - edge.start.wp);
}

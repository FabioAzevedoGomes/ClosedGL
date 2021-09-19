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

void RasterizationStrategy::DrawAlongScanlineForEdge(int index)
{
    Vertex point;
    float minX = activeEdges[index].GetMinXAfterIncrement();
    float minZ = activeEdges[index].GetMinZAfterIncrement();
    float maxX = activeEdges[index].GetMaxXAfterIncrement();
    float maxZ = activeEdges[index].GetMaxZAfterIncrement();

    for (float currentX = minX, currentZ = minZ;
         currentX <= maxX;
         currentX++, currentZ += activeEdges[index].dz / activeEdges[index].dx)
    {
        interpolateLinearlyOverEdge(activeEdges[index], point, currentX, currentY, currentZ);
        drawInterpolatedVertexToBuffer(point);
    }
}

void RasterizationStrategy::drawInterpolatedVertexToBuffer(Vertex vertex)
{
    vertex.color /= vertex.wp;
    buffers->draw(vertex.position, glm::vec4(vertex.color, 1.0f));
}

float RasterizationStrategy::distanceBetween(glm::vec4 pos1, glm::vec4 pos2)
{
    return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
}

void RasterizationStrategy::interpolateLinearlyOverEdge(Edge edge, Vertex &interpolated, float currentX, float currentY, float currentZ)
{
    float alpha = glm::dot(glm::vec4(currentX, currentY, currentZ, 1.0f) - edge.start.position, edge.end.position - edge.start.position) /
                  std::pow(distanceBetween(edge.start.position, edge.end.position), 2);

    float wa = edge.start.wp;
    float wb = edge.end.wp;
    float interpolationDenominator = ((1 - alpha) / wa) + (alpha / wb);

    interpolated.position = edge.start.position + alpha * (edge.end.position - edge.start.position);
    interpolated.position.z = edge.start.position.z + alpha * (edge.end.position.z - edge.start.position.z);
    interpolated.normal = ((edge.start.normal / wa) + alpha * ((edge.end.normal / wb) - (edge.start.normal / wa))) / interpolationDenominator;
    interpolated.color = ((edge.start.color / wa) + alpha * ((edge.end.color / wb) - (edge.start.color / wa))) / interpolationDenominator;
    interpolated.wp = 1.0f / interpolationDenominator;
}

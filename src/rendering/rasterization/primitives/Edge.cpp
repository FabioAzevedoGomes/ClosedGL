#include "Edge.hpp"

Edge::Edge()
{
    this->index = -1;
}

Edge::Edge(Vertex start, Vertex end, int index)
{
    this->start = start;
    this->end = end;

    this->currentX = start.position.x;
    this->currentZ = start.position.z;

    this->index = index;

    this->dx = std::floor(end.position.x) - std::floor(start.position.x);
    this->dy = std::floor(end.position.y) - std::floor(start.position.y);
    this->dz = std::floor(end.position.z) - std::floor(start.position.z);

    this->incX = dx / dy;
    this->incZ = dz / dy;

    if (dy == 0.0f) // Horizontal edge aligned to scanline
        this->incX = dx;

    this->justSwapped = true;
}

void Edge::Increment()
{
    if (!justSwapped)
    {
        this->currentX += incX;
        this->currentZ += incZ;
    }
    justSwapped = false;
}

float Edge::GetMinXAfterIncrement()
{
    return std::min(currentX + incX, currentX);
}

float Edge::GetMaxXAfterIncrement()
{
    return std::max(currentX + incX, currentX);
}

float Edge::GetMinZAfterIncrement()
{
    return dy == 0.0f ? 0 : std::min(currentZ + incZ, currentZ);
}

float Edge::GetMaxZAfterIncrement()
{
    return dy == 0.0f ? -1 : std::max(currentZ + incZ, currentZ);
}

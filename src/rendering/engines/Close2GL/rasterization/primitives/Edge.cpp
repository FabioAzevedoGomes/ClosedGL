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

    this->dx = end.position.x - start.position.x;
    this->dy = end.position.y - start.position.y;
    this->dz = end.position.z - start.position.z;

    this->incX = dx / dy;
    this->incZ = dz / dy;

    if (dy == 0.0f) // Horizontal edge aligned to scanline
    {
        this->incX = dx;
        this->incZ = dz/dx;
    }
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

bool Edge::IsHorizontal()
{
    return start.position.y == end.position.y;
}

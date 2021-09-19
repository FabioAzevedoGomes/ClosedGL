#pragma once

#include <cmath>
#include <iostream>
#include "Vertex.hpp"

class Edge
{
public:
    Vertex start, end;
    int index;

    float dx, dy, dz, incX, incZ;
    float currentX, currentZ;

    bool justSwapped;

    Edge();
    Edge(Vertex start, Vertex end, int index);
    void Increment();

    float GetMinXAfterIncrement();
    float GetMaxXAfterIncrement();
    float GetMinZAfterIncrement();
    float GetMaxZAfterIncrement();

    bool IsHorizontal();
};

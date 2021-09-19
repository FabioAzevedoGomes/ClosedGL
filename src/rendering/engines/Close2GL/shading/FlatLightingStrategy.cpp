#include "FlatLightingStrategy.hpp"

void FlatLightingStrategy::ShadeTriangleRelativeToCamera(Triangle &triangle, Camera &_)
{
    for (int vertex = 0; vertex < 3; vertex++)
        triangle.vertices[vertex].color = uniformMaterial.diffuseColor * uniformMaterial.diffuseIntensity;
}

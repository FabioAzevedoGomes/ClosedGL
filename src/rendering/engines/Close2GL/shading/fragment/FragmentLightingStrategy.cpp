#include "FragmentLightingStrategy.hpp"

void FragmentLightingStrategy::SetUniformMaterial(Material material)
{
    this->uniformMaterial = material;
}

void FragmentLightingStrategy::SetScreenWidthHeight(float width, float height)
{
    this->width = width;
    this->height = height;
}

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

void FragmentLightingStrategy::SetTextureEnabled(Texture *texture, bool enabled)
{
    this->texture = texture;
    this->textureEnabled = enabled;
}

void FragmentLightingStrategy::SetResamplingMode(ResamplingModes resamplingMode) 
{
    this->resamplingMode = resamplingMode;
}

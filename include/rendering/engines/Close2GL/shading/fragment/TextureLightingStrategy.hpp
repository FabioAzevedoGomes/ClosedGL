#pragma once

#include "FragmentLightingStrategy.hpp"

class TextureLightingStrategy : public FragmentLightingStrategy
{
    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &, Buffer *buffers);
};

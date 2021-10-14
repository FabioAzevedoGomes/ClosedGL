#pragma once

#include "FragmentLightingStrategy.hpp"

class PhongLightingStrategyFragment : public FragmentLightingStrategy
{
    virtual void ShadeFragmentRelativeToCamera(Vertex &, Camera &, Buffer *buffers);
};
